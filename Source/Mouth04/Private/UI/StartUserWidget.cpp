// Fill out your copyright notice in the Description page of Project Settings.


#include "StartUserWidget.h"

#include "PlayerNameWidget.h"
#include "Blueprint/UserWidget.h"
#include "SelsectPlayerWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Widget.h"
#include "Components/WrapBox.h"
#include "GameFramework/PlayerController.h"

void UStartUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CanvasPanel_Name->SetVisibility(ESlateVisibility::Hidden);
		// 绑定按钮点击事件（核心交互逻辑）
    	if (Button_Start)
    	{
    		// 点击Button_Start这个按钮,会执行这个函数
    		Button_Start->OnClicked.AddDynamic(this, &UStartUserWidget::StartGame);
    	}
	
		// 播放主菜单初始化动画（绑定的MainMenuAnim）
		if (Anim_clipping)
		{
			PlayAnimation(Anim_clipping); // UE内置函数：播放Widget动画
		}
	Button_Sure->OnClicked.AddDynamic(this, &UStartUserWidget::OncClickSure);
	Button_NewPlayer->OnClicked.AddDynamic(this, &UStartUserWidget::OncClickNewName);
	Button_ChangePlayName->OnClicked.AddDynamic(this, &UStartUserWidget::OncClickChangePlayName);
	if (!PlayerNameWidgetClass)
	{
		FString WidgetPath = TEXT("/Game/Blueprints/UMG/U_PlayerName.U_PlayerName_C");
		PlayerNameWidgetClass = LoadClass<UPlayerNameWidget>(nullptr, *WidgetPath);
		if (!PlayerNameWidgetClass)
		{
			UE_LOG(LogTemp, Error, TEXT("[StartWidget] 加载U_PlayerName蓝图失败！路径：%s"), *WidgetPath);
			return;
		}
	}
	LoadAndDisplayAllPlayers();//初始化用户列表
	RefreshSelectedImage();//初始化用户名的选中状态
	UpdatePlayerNameText();//初始化当前玩家名称
}

void UStartUserWidget::StartGame()
{
	//获取 UE 全局唯一的 GameInstance 实例，并将其转换为我们自定义的UMyGameInstance类型，从而访问其中的全局变量（如枚举、数值）
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	UE_LOG(LogTemp, Log, TEXT("UMG:点击开始游戏"));
	GameInstance->LoadAndPlaySound2D();//播放音效
	if (!ChosePlayersWidget)
	{
		TSubclassOf<USelsectPlayerWidget> WidgetClass = LoadClass<USelsectPlayerWidget>
		(
		nullptr, 
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_SelectPlayer.U_SelectPlayer_C'")
		);
		if (WidgetClass)
		{
				// 先通过GetWorld()获取PlayerController
            	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
            	if (PC)
            	{
            		ChosePlayersWidget = CreateWidget<USelsectPlayerWidget>(PC, WidgetClass);
            	}
				if (ChosePlayersWidget)
				{
					ChosePlayersWidget->AddToViewport(); // 显示到屏幕
				}
		}
	}
	else
	{
		ChosePlayersWidget->AddToViewport(); // 显示到屏幕
	}	
	GameInstance->G_WidgetChose = EG_Widget::ChosePlayer;//标记进入游戏选择界面
	this->RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("StartGame: StartUserWidget已从视口移除！"));
	
}

//进入玩家切换页面
void UStartUserWidget::OncClickChangePlayName()
{
	CanvasPanel_Name->SetVisibility(ESlateVisibility::Visible);
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->LoadAndPlaySound2D();//播放音效
	GI->StartWidgetInstance = this; // 存储U_Start实例指针
	PauseGame();
	PlayAnimation(Anim_clipping);
}

//新建玩家信息和存档
void UStartUserWidget::OncClickNewName()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->LoadAndPlaySound2D();//播放音效
	// 读取输入框文本并转换为FString 
	FString InputName = EditableTextBox_Name->GetText().ToString();
	// 校验输入合法性 
	if (!ValidatePlayerName(InputName))
	{
		// 校验失败：弹出提示框（蓝图中也可通过EventDispatch实现）
		UKismetSystemLibrary::PrintString(this, TEXT("昵称输入不合法！\n请输入1-5个汉字"), true, true, FLinearColor::Red, 3.0f);
		return;
	}
	// 存储输入的昵称
	PlayerNickName = InputName;
	// 调用新增玩家函数，传入输入的昵称
	int32 NewPlayerID = GI->AddNewPlayer(InputName);
	// 清空输入框
	EditableTextBox_Name->SetText(FText::GetEmpty());
	
	//刷新显示框
	LoadAndDisplayAllPlayers();
	GI->AutoSaveCurrentPlayer();//更新存档
	RefreshSelectedImage();    // 刷新选中图片
}

void UStartUserWidget::OncClickSure()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	bool LoadDataByChoseID = GI->SetCurrentSelectedPlayer(GI->CurrentSelectedPlayerID);
	if (!LoadDataByChoseID)
	{
		UE_LOG(LogTemp, Error, TEXT("[StartWidget] 加载PlayerID对应的关卡数据失败"));
	}
	UpdatePlayerNameText();
	GI->LoadAndPlaySound2D();//播放音效
	ResumeGame();
	PlayAnimation(Anim_clipping);
	CanvasPanel_Name->SetVisibility(ESlateVisibility::Hidden);
}

void UStartUserWidget::UpdatePlayerNameText()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!TextBlock_ChangeName || !GI)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartUserWidget: 文本控件/GameInstance无效，无法更新玩家名称"));
		return;
	}
	// 1. 获取当前选中的PlayerID和对应的名称
	int32 CurrentID = GI->CurrentSelectedPlayerID;
	FString PlayerName = TEXT("默认玩家"); // 默认值
	// 2. 从GameInstance的映射表中查找名称
	for (const FPlayerSaveData& PlayerData : GI->PlayerSaveDatas)
	{
		if (PlayerData.PlayerID==CurrentID)
		{
			PlayerName=PlayerData.PlayerName;
		}
	}
	// 3. 设置文本显示（FString转FText，UE文本控件需要FText类型）
	TextBlock_ChangeName->SetText(FText::FromString(PlayerName));
}

void UStartUserWidget::LoadAndDisplayAllPlayers()
{
	// 清空原有子项和缓存
	WrapBox_Name->ClearChildren();
	SpawnedPlayerWidgets.Empty();

	// 获取GameInstance和玩家数据
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GameInstance || GameInstance->PlayerSaveDatas.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[StartWidget] 无玩家数据可显示"));
		return;
	}

	// 遍历玩家数据，动态生成U_PlayerName子项
	for (const FPlayerSaveData& PlayerData : GameInstance->PlayerSaveDatas)
	{
		// 跳过无效玩家（bCanUse=false）
		if (!PlayerData.bCanUse)
		{
			continue;
		}

		// 1. 创建U_PlayerName蓝图实例
		UPlayerNameWidget* ChildWidget = CreateWidget<UPlayerNameWidget>(GetWorld(), PlayerNameWidgetClass);
		if (!ChildWidget)
		{
			UE_LOG(LogTemp, Error, TEXT("[StartWidget] 创建U_PlayerName实例失败！"));
			continue;
		}

		// 2. 调用SetPlayerData，设置昵称和ID
		ChildWidget->SetPlayerData(PlayerData.PlayerName, PlayerData.PlayerID);
		
		// 4. 添加到WrapBox容器
		WrapBox_Name->AddChildToWrapBox(ChildWidget);

		// 5. 存储实例，防止被GC回收
		SpawnedPlayerWidgets.Add(ChildWidget);

		UE_LOG(LogTemp, Warning, TEXT("[StartWidget] 生成玩家项：ID=%d，昵称=%s"), PlayerData.PlayerID, *PlayerData.PlayerName);
	}
}

void UStartUserWidget::RefreshSelectedImage()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!GI) { return; }

	// 遍历所有子Widget，对比ID控制显隐
	for (UPlayerNameWidget* ChildWidget : SpawnedPlayerWidgets)
	{
		if (!ChildWidget) { continue; }
		bool bShouldShow = (ChildWidget->PlayerID_X == GI->CurrentSelectedPlayerID);
		ChildWidget->SetChoseImageVisibility(bShouldShow);
	}
}

//暂停游戏
void UStartUserWidget::PauseGame()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("【StartWidget】无法暂停游戏：世界上下文无效！"));
		return;
	}

	// 核心函数：设置游戏为暂停状态
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeUIOnly()); // 仅允许 UI 输入，禁止游戏操作
	}
}
//继续游戏
void UStartUserWidget::ResumeGame()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("【StartWidget】无法恢复游戏：世界上下文无效！"));
		return;
	}

	// 核心函数：设置游戏为运行状态
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeGameOnly()); // 仅允许游戏输入，禁止 UI 操作
	}

}

bool UStartUserWidget::ValidatePlayerName(const FString& InputName)
{
	// 1. 空值校验
	if (InputName.IsEmpty() || InputName.TrimStartAndEnd().IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[StartWidget] 昵称校验失败：输入为空"));
		return false;
	}
	// 2. 长度校验（12个字符以内）
	int32 CharLength = InputName.Len();
	if (CharLength > 12)
	{
		UE_LOG(LogTemp, Warning, TEXT("[StartWidget] 昵称校验失败：长度超过6个汉字（当前长度：%d）"), CharLength / 2);
		return false;
	}
	return true;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUserWidget.h"
// UE内置头文件：包含UI动画、按钮事件相关函数
#include "Components/Button.h"
#include "Players/ABoxBot.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HorizontalBox.h"
#include "Math/UnrealMathUtility.h"
#include "GameInstance/MyGameInstance.h"
#include "ExitWidget.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/UserWidget.h"

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();// 必须调用父类的NativeConstruct，保证基类逻辑执行
	
	SittingWidgetClass = LoadClass<UUserWidget>(
	nullptr, // 类加载器，无需指定，传 nullptr 即可
	TEXT("/Game/Blueprints/UMG/U_Sitting.U_Sitting_C") );
	// 验证蓝图是否加载成功
	if (!IsValid(SittingWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("【NewWidget】加载 U_Sitting 蓝图失败！请检查路径是否正确（需加 .xxx_C 后缀）！"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("【NewWidget】U_Sitting 蓝图加载成功！"));
	}
	
	// 绑定按钮点击事件（核心交互逻辑）
	if (ShowSettingBtn)
	{
		// 点击ShowSettingBtn这个按钮,会执行ShowSettingWidget这个函数
		ShowSettingBtn->OnClicked.AddDynamic(this, &UMyUserWidget::ShowSettingWidget);
	}
	CanvasPanel_Setting->SetVisibility(ESlateVisibility::Hidden);
	//获取 UE 全局唯一的 GameInstance 实例，并将其转换为我们自定义的UMyGameInstance类型，从而访问其中的全局变量（如枚举、数值）
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UI中读取 GameInstance无效!!"));
		return;
	}
		
	// 订阅委托
	GI=GameInstance;
	GI->OnP1RemainingBoxNumberChanged.AddDynamic
	(
		this,
		&UMyUserWidget::OnRemainingBoxNumberChanged
	);
	GI->OnP2RemainingBoxNumberChanged.AddDynamic
	(
		this,
		&UMyUserWidget::OnP2RemainingBoxNumberChanged
	);
		
	//初始化玩家1的数值和图标
	FText P1MaxBoxNumberText = FText::FromString(FString::Printf(TEXT("%d"), GameInstance->G_P1MaxBoxNumber));
	SetTextBlockContent(TextBlock_P1MaxNum,P1MaxBoxNumberText);
	//  初始化显示当前剩余箱子数（避免UI初始为空）
	UpdateP1RemainingBoxNumberText(GI->GetP1RemainingBoxNumber());
	FText P1RemainingBoxNumberText = FText::FromString(FString::Printf(TEXT("%d"), GameInstance->G_P1RemainingBoxNumber));	
	SetTextBlockContent(TextBlock_P1CanUseNum,P1RemainingBoxNumberText);
	// 读取GI数据,更新玩家头像UI
	BindPlayerTexturesToImages(GI->G_P1SelectedClass, Image_P1.Get(), Image_P1Box.Get());
	
	//初始化玩家2的数值和图标
	if (GameInstance->bIsTwoPlayerMode==false)
	{
		HorizontalBox_P2->SetVisibility(ESlateVisibility::Hidden);//隐藏P2条形框
	}
	else
	{
		HorizontalBox_P2->SetVisibility(ESlateVisibility::Visible);
		FText P2MaxBoxNumberText = FText::FromString(FString::Printf(TEXT("%d"), GameInstance->G_P2MaxBoxNumber));
		SetTextBlockContent(TextBlock_P2MaxNum,P2MaxBoxNumberText);
		UpdateP2RemainingBoxNumberText(GI->GetP2RemainingBoxNumber());
		// 读取GI数据,更新玩家头像UI
		BindPlayerTexturesToImages(GI->G_P2SelectedClass, Image_P2.Get(), Image_P2Box.Get());
	}
	GI->bIsShowSetting=false;
}

// UI销毁：取消委托订阅（可选，AddUObject已处理，但显式取消更安全）
void UMyUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(GI))
	{
		GI->OnP1RemainingBoxNumberChanged.RemoveAll(this);
	}
}

void UMyUserWidget::ShowSettingWidget()
{
	if (GI->bIsShowSetting==true) return;
	UE_LOG(LogTemp, Log, TEXT("点击setting按钮"));
	UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("【NewWidget】世界上下文无效，无法创建 U_Sitting 实例！"));
		return;
	}
	// 创建 U_Sitting 实例（每次点击都创建新实例）
	UUserWidget* NewSittingWidget = CreateWidget<UUserWidget>(
		CurrentWorld,
		SittingWidgetClass
	);
	if (!IsValid(NewSittingWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("创建 U_Sitting 实例失败！"));
		return;
	}
	PauseGame();
	NewSittingWidget->AddToViewport(10); 
	GI->bIsShowSetting=true;
	UE_LOG(LogTemp, Log, TEXT("U_Sitting 已成功添加到游戏视口！"));
}

// ========== 通用函数：修改任意文本块内容 ==========
void UMyUserWidget::SetTextBlockContent(UTextBlock* TargetTextBlock, const FText& NewText)
{
	// 安全判断：目标文本块为空则打印错误
	if (!IsValid(TargetTextBlock))
	{
		UE_LOG(LogTemp, Error, TEXT("UMG:目标文本块为空，无法修改文本内容！"));
		return;
	}

	// 修改文本内容
	TargetTextBlock->SetText(NewText);
	// 打印日志：包含文本块名称（方便定位是哪个控件）
	UE_LOG(LogTemp, Log, TEXT("UMG:文本块[%s]内容修改为：%s"), 
		*TargetTextBlock->GetName(), // 获取控件名称
		*NewText.ToString());
}

// ========== 通用函数：修改任意图片控件的纹理 ==========
void UMyUserWidget::SetImageByPath(UImage* TargetImage, const FString& ImagePath)
{
	// 安全判断：目标图片控件为空
	if (!IsValid(TargetImage))
	{
		UE_LOG(LogTemp, Error, TEXT("UMG:目标图片控件为空，无法修改图片！"));
		return;
	}

	// 空路径：清空图片
	if (ImagePath.IsEmpty())
	{
		TargetImage->SetBrushFromTexture(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("UMG:图片路径为空，清空图片控件[%s]"), *TargetImage->GetName());
		return;
	}

	// 加载纹理资源
	UTexture2D* TargetTexture = LoadObject<UTexture2D>(
		nullptr,
		*ImagePath,
		nullptr,
		LOAD_None
	);

	if (IsValid(TargetTexture))
	{
		TargetImage->SetBrushFromTexture(TargetTexture);
		UE_LOG(LogTemp, Log, TEXT("UMG:图片控件[%s]成功加载纹理：%s"), 
			*TargetImage->GetName(), 
			*ImagePath);
	}
	else
	{
		TargetImage->SetBrushFromTexture(nullptr);
		UE_LOG(LogTemp, Error, TEXT("UMG:图片控件[%s]加载纹理失败！路径：%s"), 
			*TargetImage->GetName(), 
			*ImagePath);
	}
}


// 委托处理函数：接收剩余箱子数并更新UI
void UMyUserWidget::OnRemainingBoxNumberChanged(int32 NewNumber)
{
	UpdateP1RemainingBoxNumberText(NewNumber);
}

void UMyUserWidget::OnP2RemainingBoxNumberChanged(int32 NewNumber)
{
	UpdateP2RemainingBoxNumberText(NewNumber);
}

// 现有箱子数更新函数
void UMyUserWidget::UpdateP1RemainingBoxNumberText(int32 NewNumber)
{
	if (IsValid(TextBlock_P1CanUseNum))
	{
		// 格式化文本（可根据UI需求调整文案）
		FText DisplayText = FText::FromString(FString::Printf(TEXT("%d"), NewNumber));
		TextBlock_P1CanUseNum->SetText(DisplayText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MyUserWidget: TextBlock_P1CanUseNum控件为空"));
	}
}

void UMyUserWidget::UpdateP2RemainingBoxNumberText(int32 NewNumber)
{
	if (IsValid(TextBlock_P2CanUseNum))
	{
		// 格式化文本（可根据UI需求调整文案）
		FText DisplayText = FText::FromString(FString::Printf(TEXT("%d"), NewNumber));
		TextBlock_P2CanUseNum->SetText(DisplayText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MyUserWidget: TextBlock_P2CanUseNum控件为空"));
	}
}

// 工具函数：通用纹理绑定函数（适配任意玩家）
void UMyUserWidget::BindPlayerTexturesToImages(TSubclassOf<AABoxBot> PlayerSelectedClass, UImage* TargetImage_Player, UImage* TargetImage_Box)
{
	// 安全检查：传入的 Image 控件不能为空
	if (!IsValid(TargetImage_Player) || !IsValid(TargetImage_Box))
	{
		UE_LOG(LogTemp, Error, TEXT("通用绑定失败：玩家图片控件为空！"));
		return;
	}

	// 安全检查：玩家选择的类不能为空
	if (!PlayerSelectedClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("通用绑定失败：玩家选择的类无效，清空对应图片控件"));
		// 清空两个 Image 控件
		SetImageByPath(TargetImage_Player, TEXT(""));
		SetImageByPath(TargetImage_Box, TEXT(""));
		return;
	}

	// 获取 CDO 提取纹理
	AABoxBot* ClassDefaultObject = PlayerSelectedClass->GetDefaultObject<AABoxBot>();
	if (!ClassDefaultObject)
	{
		UE_LOG(LogTemp, Error, TEXT("通用绑定失败：获取 AABoxBot CDO 失败！"));
		return;
	}

	// 绑定 PlayerType 到 TargetImage_Player
	if (UTexture2D* PlayerTexture = ClassDefaultObject->PlayerType)
	{
		SetImageByPath(TargetImage_Player, PlayerTexture->GetPathName());
	}
	else
	{
		SetImageByPath(TargetImage_Player, TEXT(""));
		UE_LOG(LogTemp, Warning, TEXT("通用绑定：PlayerType 纹理为空，清空玩家图片控件"));
	}

	// 绑定 BoxType 到 TargetImage_Box
	if (UTexture2D* BoxTexture = ClassDefaultObject->BoxType)
	{
		SetImageByPath(TargetImage_Box, BoxTexture->GetPathName());
	}
	else
	{
		SetImageByPath(TargetImage_Box, TEXT(""));
		UE_LOG(LogTemp, Warning, TEXT("通用绑定：BoxType 纹理为空，清空玩家箱子图片控件"));
	}
}


// 封装暂停游戏逻辑
void UMyUserWidget::PauseGame()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("【ExitWidget】无法暂停游戏：世界上下文无效！"));
		return;
	}

	// 核心函数：设置游戏为暂停状态
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeUIOnly()); // 仅允许 UI 输入，禁止游戏操作
	}

	UE_LOG(LogTemp, Log, TEXT("【ExitWidget】游戏已暂停！"));
}

// 封装恢复游戏逻辑
void UMyUserWidget::ResumeGame()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("【ExitWidget】无法恢复游戏：世界上下文无效！"));
		return;
	}

	// 核心函数：设置游戏为运行状态
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeGameOnly()); // 仅允许游戏输入，禁止 UI 操作
	}

	UE_LOG(LogTemp, Log, TEXT("【ExitWidget】游戏已恢复运行！"));
}
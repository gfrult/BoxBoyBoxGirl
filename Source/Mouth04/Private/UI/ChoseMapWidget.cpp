// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoseMapWidget.h"
#include "Misc/Paths.h"
#include "LockWidget.h"
#include "SelsectPlayerWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Mouth04/MainMenuGameMode.h"


void UChoseMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)//初始化显示界面,单人或双人地图
    {
    	if (GI->bIsTwoPlayerMode)//双人地图
    	{
    		WidgetSwitcher_PlayerN->SetActiveWidgetIndex(1);
    		ModeStr="Coop_";
		    for (int32 i = 1; i <= 6; ++i)
		    {
		    	InitializeMapLockP2(i);//初始化关卡的锁定状态
		    }
    		ChoseMapAnim(0);//默认没有选择关卡
    	}
    	else//单人地图界面
    	{
    		ModeStr="Solo_";
    		WidgetSwitcher_PlayerN->SetActiveWidgetIndex(0);
    	}
	}
	Button_Map1->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedTowMap1);
	Button_Map2->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedTowMap2);
	Button_Map21->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedTowMap2);
	Button_Map3->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedTowMap3);
	Button_Map4->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedTowMap4);
	Button_Map41->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedTowMap4);
	Button_Map5->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedTowMap5);
	Button_Map6->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedTowMap6);
	Button_Solo1->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedOneSolo1);
	Button_Solo2->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedOneSolo2);
	Button_Solo3->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedOneSolo3);
	if (Button_InMap)
	{
		Button_InMap->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedInMap);
		Button_InMap->SetRenderOpacity(0.3);
	}
	if (Button_ChosePlayer)
	{
		Button_ChosePlayer->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedChosePlayer);
		Button_ChosePlayer->OnHovered.AddDynamic(this, &UChoseMapWidget::OnHoveredChosePlayer);
	}
	
}

//初始化双人关卡的解锁状态
void UChoseMapWidget::InitializeMapLockP2(int32 MapIndex)
{
	CombineMapName(MapIndex);//更新FName MapName
	ULockWidget* U_MapLock =nullptr;
	TObjectPtr<UImage> Image_TowGray=nullptr;
	TObjectPtr<UWidgetAnimation> Anim_UnlockMap=nullptr;
	switch (MapIndex)
	{
		case 1:
			U_MapLock =U_MapLock1;
			Anim_UnlockMap=Anim_UnlockMap1;
			Image_TowGray=Image_TowGray1;
			break;
		case 2:
			U_MapLock =U_MapLock2;
			Anim_UnlockMap=Anim_UnlockMap2;
			Image_TowGray=Image_TowGray2;
			break;	
		case 3:
			U_MapLock =U_MapLock3;
			Anim_UnlockMap=Anim_UnlockMap3;
			Image_TowGray=Image_TowGray3;
			break;	
		case 4:
			U_MapLock =U_MapLock4;
			Anim_UnlockMap=Anim_UnlockMap4;
			Image_TowGray=Image_TowGray4;
			break;	
		case 5:
			U_MapLock =U_MapLock5;
			Anim_UnlockMap=Anim_UnlockMap5;
			Image_TowGray=Image_TowGray5;
			break;	
		case 6:
			U_MapLock =U_MapLock6;
			Anim_UnlockMap=Anim_UnlockMap6;
			Image_TowGray=Image_TowGray6;
			break;	
		default:
			break;
	}
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI->GetLevelStatus(MapName)==ELevelStatus::Unlocked)//解锁
	{
		U_MapLock->SetVisibility(ESlateVisibility::Hidden);//隐藏锁
		Image_TowGray->SetVisibility(ESlateVisibility::Hidden);//隐藏灰色
		UE_LOG(LogTemp,Log, TEXT("UI初始化:%s :Unlocked，已解锁"),*MapName.ToString());
	}
	else if (GI->GetLevelStatus(MapName)==ELevelStatus::FirstUnlocked)
	{
		U_MapLock->SetVisibility(ESlateVisibility::Visible);
		Image_TowGray->SetVisibility(ESlateVisibility::Visible);
		U_MapLock->Unlock();//播放解锁动画
		PlayAnimation(Anim_UnlockMap);
		GI->MarkLevelAsSeen(MapName);//将状态标记为已解锁
		UE_LOG(LogTemp,Log, TEXT("UI初始化:%s :FirstUnlocked，初次解锁"),*MapName.ToString());
	}
	else if (GI->GetLevelStatus(MapName)==ELevelStatus::Locked)//锁定
	{
		U_MapLock->SetVisibility(ESlateVisibility::Visible);//显示锁
		Image_TowGray->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp,Log, TEXT("UI初始化:%s :Locked，锁定"),*MapName.ToString());
	}
	else
	{
		UE_LOG(LogTemp,Warning, TEXT("UI初始化:%s :存疑的状态!!"),*MapName.ToString());
	}
}

//点击进入关卡按钮
void UChoseMapWidget::OnClickedInMap()
{
	UE_LOG(LogTemp, Log, TEXT("UMG：点击进入关卡,当前bNextInMap=%s,地图名称=%s"),
		bNextInMap ? TEXT("true") : TEXT("false"),  
		*MapName.ToString()                          
		);
	
	if (bNextInMap)
	{
		// 1. 先获取MainMenuGameMode
        AMainMenuGameMode* MyGM = GetCustomGameMode();
        if (!MyGM)
        {
        	return; // 获取失败则直接返回，避免空指针
        }
        //2.调用函数
        //MyGM->StartGameLevel("M_Box");//注意这里默认调用的是map1
        UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
        if (GI->GetLevelStatus(MapName)!=ELevelStatus::Locked)//先判断是否解锁
        {
        	GI->SetMaxBox(MapName);//设置最大盒子数
            MyGM->StartGameLevel(MapName);//切换关卡
        }
	}
}

void UChoseMapWidget::OnClickedChosePlayer()
{
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
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());	
	GI->G_WidgetChose = EG_Widget::ChosePlayer;//标记进入游戏选择界面,此步骤可以省略
	this->RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("UMG: chose Map 已从视口移除！"));
}

void UChoseMapWidget::OnHoveredChosePlayer()
{
	PlayAnimation(Anim_ChangePlayerMod);
}

//选择地图按钮的逻辑
void UChoseMapWidget::OnClickedMapBotton(int32 MapIndex)
{
	UE_LOG(LogTemp,Log, TEXT("UMG:点击地图%d"),MapIndex);
	CombineMapName(MapIndex);//更新FName MapName
	ChoseMapAnim(MapIndex);//显示选择高亮状态
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI->GetLevelStatus(MapName)==ELevelStatus::Locked)
	{
		bNextInMap=false;
		ShakeLockedMap(MapIndex);
	}
	else 
	{
		PlayAnimation(Anim_Next);
		Button_InMap->SetRenderOpacity(MapIndex);
		bNextInMap = true;
	}
}

void UChoseMapWidget::OnClickedTowMap1()
{
	OnClickedMapBotton(1);
}

void UChoseMapWidget::OnClickedTowMap2()
{
	OnClickedMapBotton(2);
}

void UChoseMapWidget::OnClickedTowMap3()
{
	OnClickedMapBotton(3);
}

void UChoseMapWidget::OnClickedTowMap4()
{
	OnClickedMapBotton(4);
}

void UChoseMapWidget::OnClickedTowMap5()
{
	OnClickedMapBotton(5);
}

void UChoseMapWidget::OnClickedTowMap6()
{
	OnClickedMapBotton(6);
}

void UChoseMapWidget::OnClickedOneSolo1()
{
	OnClickedMapBotton(1);
}

void UChoseMapWidget::OnClickedOneSolo2()
{
	OnClickedMapBotton(2);
}

void UChoseMapWidget::OnClickedOneSolo3()
{
	OnClickedMapBotton(3);
}

// 工具函数：安全获取自定义GameMode实例
AMainMenuGameMode* UChoseMapWidget::GetCustomGameMode()
{
	// 步骤1：获取当前World（Widget必须在世界中才能获取GameMode）
	UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("CleanTestWidget：获取World失败！"));
		return nullptr;
	}

	// 步骤2：获取基础GameMode（单机/服务端用GetAuthGameMode）
	AGameModeBase* BaseGM = CurrentWorld->GetAuthGameMode();
	// 客户端兼容写法（联网游戏客户端GameMode只读）：
	// AGameModeBase* BaseGM = CurrentWorld->GetGameMode();

	// 步骤3：向下转型为自定义GameMode（必须用Cast，避免崩溃）
	AMainMenuGameMode* CustomGM = Cast<AMainMenuGameMode>(BaseGM);
	if (!CustomGM)
	{
		UE_LOG(LogTemp, Error, TEXT("CleanTestWidget：GameMode类型转换失败！请检查关卡GameMode设置"));
		return nullptr;
	}
	return CustomGM;
}

// 工具函数：MapName字符串拼接
void UChoseMapWidget::CombineMapName(int32 MapIndex)
{
	FString IndexStr = FString::Printf(TEXT("%02d"), MapIndex);
	// 拼接字符串并转换为FName
	FString CombinedStr = ModeStr + IndexStr;
	MapName = FName(CombinedStr);
}

//未解锁地图的抖动动画
void UChoseMapWidget::ShakeLockedMap(int32 MapIndex)
{
	ULockWidget* TargetLockWidget = nullptr;

	switch (MapIndex)
	{
	case 1:
		TargetLockWidget = U_MapLock1;
		break;
	case 2:
		TargetLockWidget = U_MapLock2;
		break;
	case 3:
		TargetLockWidget = U_MapLock3;
		break;
	case 4:
		TargetLockWidget = U_MapLock4;
		break;
	case 5:
		TargetLockWidget = U_MapLock5;
		break;	
	case 6:
		TargetLockWidget = U_MapLock6;
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("无效的MapIndex：%d"), MapIndex);
		return;
	}

	if (TargetLockWidget && IsValid(TargetLockWidget))
	{
		Button_InMap->SetRenderOpacity(0.3);
		TargetLockWidget->LockShake();
		//UE_LOG(LogTemp, Log, TEXT("图片%d点击，调用U_MapLock%d的LockShake"), MapIndex, MapIndex);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("U_MapLock%d指针无效！"), MapIndex);
	}
}

//选择地图时的标记动画
void UChoseMapWidget::ChoseMapAnim(int32 MapIndex)
{
	Image_TwoM1->SetVisibility(ESlateVisibility::Hidden);
    Image_TwoM2->SetVisibility(ESlateVisibility::Hidden);
    Image_TwoM3->SetVisibility(ESlateVisibility::Hidden);
    Image_TwoM4->SetVisibility(ESlateVisibility::Hidden);
    Image_TwoM5->SetVisibility(ESlateVisibility::Hidden);
    Image_TwoM6->SetVisibility(ESlateVisibility::Hidden);
    switch (MapIndex)
    {
        case 1:
        Image_TwoM1->SetVisibility(ESlateVisibility::Visible);
        break;
        case 2:
        Image_TwoM2->SetVisibility(ESlateVisibility::Visible);
        break;
        case 3:
        Image_TwoM3->SetVisibility(ESlateVisibility::Visible);
        break;		
        case 4:
        Image_TwoM4->SetVisibility(ESlateVisibility::Visible);
        break;
        case 5:
        Image_TwoM5->SetVisibility(ESlateVisibility::Visible);
        break;
        case 6:
        Image_TwoM6->SetVisibility(ESlateVisibility::Visible);
        break;
        default:
        UE_LOG(LogTemp, Log, TEXT("UMG:map选择界面:超过范围或初始化选择状态"));
        break;
    }
	PlayAnimation(Anim_ChoseShare);
}


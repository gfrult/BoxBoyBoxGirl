// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoseMapWidget.h"
#include "Misc/Paths.h"
#include "LockWidget.h"
#include "MapTransitionWidget.h"
#include "StarWidget.h"
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
		    	InitializeMapLock(i);//初始化关卡的锁定状态
		    }
    		ChoseMapAnim(0);//默认没有选择关卡
    	}
    	else//单人地图界面
    	{
    		ModeStr="Solo_";
    		WidgetSwitcher_PlayerN->SetActiveWidgetIndex(0);
    		InitializeMapLock(1);
    		InitializeMapLock(2);
    		InitializeMapLock(3);
    		ChoseMapAnim(0);//默认没有选择关卡
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
	Button_Solo11->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedOneSolo1);
	Button_Solo2->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedOneSolo2);
	Button_Solo21->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedOneSolo2);
	Button_Solo3->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedOneSolo3);
	
	TransitionWidgetClass=LoadClass<UUserWidget>(
		nullptr, // 类加载器，无需指定，传 nullptr 即可
	TEXT("/Game/Blueprints/UMG/U_MapTransition.U_MapTransition_C") );
	if (!IsValid(TransitionWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("加载 U_MapTransition 蓝图失败！请检查路径是否正确！"));
	}
	
	
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
	GI->bIsShowSetting=false;
	SittingWidgetClass = LoadClass<UUserWidget>(
		nullptr, // 类加载器，无需指定，传 nullptr 即可
	TEXT("/Game/Blueprints/UMG/U_Sitting.U_Sitting_C") );
	// 验证蓝图是否加载成功
	if (!IsValid(SittingWidgetClass))
	{
		UE_LOG(LogTemp, Error, TEXT("加载 U_Sitting 蓝图失败！请检查路径是否正确！"));
	}
	Button_Setting->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedSetting);
	
}

//初始化关卡的解锁状态
void UChoseMapWidget::InitializeMapLock(int32 MapIndex)
{
	CombineMapName(MapIndex);//更新FName MapName
	ULockWidget* U_MapLock =nullptr;
	UStarWidget* U_Star =nullptr;
	TObjectPtr<UImage> Image_TowGray=nullptr;
	TObjectPtr<UWidgetAnimation> Anim_UnlockMap=nullptr;
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	switch (MapIndex)
	{
		case 1:
			if (GI->bIsTwoPlayerMode)
			{
				U_MapLock =U_MapLock1;
	            Anim_UnlockMap=Anim_UnlockMap1;
	            Image_TowGray=Image_TowGray1;
	            U_Star=U_Star_1;
			}
			else
			{
				U_MapLock =U_MapLock_O1;
				Anim_UnlockMap=Anim_UnlockSolo1;
				Image_TowGray=Image_OneGray1;
				U_Star=U_Star_O1;
			}
			break;
		case 2:
			if (GI->bIsTwoPlayerMode)
			{
				U_MapLock =U_MapLock2;
				Anim_UnlockMap=Anim_UnlockMap2;
				Image_TowGray=Image_TowGray2;
				U_Star=U_Star_2;			
			}
			else
			{
				U_MapLock =U_MapLock_O2;
				Anim_UnlockMap=Anim_UnlockSolo2;
				Image_TowGray=Image_OneGray2;
				U_Star=U_Star_O2;
			}
			break;	
		case 3:
			if (GI->bIsTwoPlayerMode)
			{
				U_MapLock =U_MapLock3;
    			Anim_UnlockMap=Anim_UnlockMap3;
    			Image_TowGray=Image_TowGray3;
    			U_Star=U_Star_3;			
			}
			else
			{
				U_MapLock =U_MapLock_O3;
				Anim_UnlockMap=Anim_UnlockSolo3;
				Image_TowGray=Image_OneGray3;
				U_Star=U_Star_O3;
			}
			break;	
		case 4:
			U_MapLock =U_MapLock4;
			Anim_UnlockMap=Anim_UnlockMap4;
			Image_TowGray=Image_TowGray4;
			U_Star=U_Star_4;
			break;	
		case 5:
			U_MapLock =U_MapLock5;
			Anim_UnlockMap=Anim_UnlockMap5;
			Image_TowGray=Image_TowGray5;
			U_Star=U_Star_5;
			break;	
		case 6:
			U_MapLock =U_MapLock6;
			Anim_UnlockMap=Anim_UnlockMap6;
			Image_TowGray=Image_TowGray6;
			U_Star=U_Star_6;
			break;	
		default:
			break;
	}

	if (GI->GetLevelStatus(MapName)==ELevelStatus::Unlocked)//解锁
	{
		U_MapLock->SetVisibility(ESlateVisibility::Hidden);//隐藏锁
		Image_TowGray->SetVisibility(ESlateVisibility::Hidden);//隐藏灰色
		U_Star->SetStarNumber(GI->GetStarNum(MapName));
		UE_LOG(LogTemp,Log, TEXT("UI初始化:%s :Unlocked，已解锁"),*MapName.ToString());
	}
	else if (GI->GetLevelStatus(MapName)==ELevelStatus::FirstUnlocked)
	{
		U_MapLock->SetVisibility(ESlateVisibility::Visible);
		Image_TowGray->SetVisibility(ESlateVisibility::Visible);
		U_MapLock->Unlock();//播放解锁动画
		PlayAnimation(Anim_UnlockMap);
		GI->MarkLevelAsSeen(MapName);//将状态标记为已解锁
		U_Star->SetStarNumber(0);
		UE_LOG(LogTemp,Log, TEXT("UI初始化:%s :FirstUnlocked，初次解锁"),*MapName.ToString());
	}
	else if (GI->GetLevelStatus(MapName)==ELevelStatus::Locked)//锁定
	{
		U_MapLock->SetVisibility(ESlateVisibility::Visible);//显示锁
		Image_TowGray->SetVisibility(ESlateVisibility::Visible);
		U_Star->SetStarNumber(0);
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
	UE_LOG(LogTemp, Log, TEXT("UMG：点击进入关卡按钮,当前bNextInMap=%s,地图名称=%s"),
		bNextInMap ? TEXT("true") : TEXT("false"),  
		*MapName.ToString()                          
		);
	
	if (bNextInMap)
	{
        UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
		FString OtherSoundPath = TEXT("/Game/MyBoxGame/Sounds/SoundEffects/UI/Sure_Sound.Sure_Sound");
		GI->LoadAndPlaySound2D(OtherSoundPath);//播放确认的音效
        if (GI->GetLevelStatus(MapName)!=ELevelStatus::Locked)//先判断是否解锁
        {
        	GI->SetMaxBox(MapName);//设置最大盒子数
        	//播放转场动画并加载新关卡
        	MapTransitionAndLoadNewMap();
        }
	}
	else
	{
		UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
		FString OtherSoundPath = TEXT("/Game/MyBoxGame/Sounds/SoundEffects/UI/Miss_Sound.Miss_Sound");
		GI->LoadAndPlaySound2D(OtherSoundPath);
	}
}

void UChoseMapWidget::OnClickedSetting()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI->bIsShowSetting==true) return;
	UE_LOG(LogTemp, Log, TEXT("点击setting按钮"));
	UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("【NewWidget】世界上下文无效，无法创建 U_Sitting 实例！"));
		return;
	}
	// 创建 U_Sitting 实例
	UUserWidget* NewSittingWidget = CreateWidget<UUserWidget>(
		CurrentWorld,
		SittingWidgetClass
	);
	if (!IsValid(NewSittingWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("创建 U_Sitting 实例失败！"));
		return;
	}
	NewSittingWidget->AddToViewport(10); 
	GI->bIsShowSetting=true;
	UE_LOG(LogTemp, Log, TEXT("U_Sitting 已成功添加到游戏视口！"));
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
	GI->LoadAndPlaySound2D();
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
		FString OtherSoundPath = TEXT("/Game/MyBoxGame/Sounds/SoundEffects/UI/Locked_Sound.Locked_Sound");
		GI->LoadAndPlaySound2D(OtherSoundPath);//播放锁住的音效
		bNextInMap=false;
		ShakeLockedMap(MapIndex);
	}
	else 
	{
		GI->LoadAndPlaySound2D();//正常点击音效
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

void UChoseMapWidget::MapTransitionAndLoadNewMap()
{
	UWorld* CurrentWorld = GetWorld();
	if (!CurrentWorld) return;
	// 创建 U_MapTransition实例（每次点击都创建新实例）
	UMapTransitionWidget* MapTransitionInstance = CreateWidget<UMapTransitionWidget>(
		CurrentWorld,
		TransitionWidgetClass
	);
	if (!IsValid(MapTransitionInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("创建 U_MapTransition例失败！"));
		return;
	}
	// 将过渡UI添加到视口最上方
	if (!MapTransitionInstance->IsInViewport())
	{
		MapTransitionInstance->AddToViewport(1000); // TopZOrder=1000，确保覆盖所有其他UI
	}
	// 确保过渡UI可见（防止蓝图中默认隐藏）
	MapTransitionInstance->SetVisibility(ESlateVisibility::Visible);
	
	if (IsValid(CurrentWorld))
	{
		// 先清除旧定时器，避免重复加载
		CurrentWorld->GetTimerManager().ClearTimer(DelayLoadTimerHandle);
		CurrentWorld->GetTimerManager().SetTimer(
			DelayLoadTimerHandle,
			FTimerDelegate::CreateUObject(this, &UChoseMapWidget::OnDelayLoadNewLevel),
			0.5f,
			false
		);
	}
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
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	switch (MapIndex)
	{
	case 1:
		TargetLockWidget = (GI->bIsTwoPlayerMode)? U_MapLock1:U_MapLock_O1;
		break;
	case 2:
		TargetLockWidget = (GI->bIsTwoPlayerMode)? U_MapLock2:U_MapLock_O2;
		break;
	case 3:
		TargetLockWidget = (GI->bIsTwoPlayerMode)? U_MapLock3:U_MapLock_O3;
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
	Image_OneM1->SetVisibility(ESlateVisibility::Hidden);
	Image_OneM2->SetVisibility(ESlateVisibility::Hidden);
	Image_OneM3->SetVisibility(ESlateVisibility::Hidden);
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    switch (MapIndex)
    {
        case 1:
        if (GI->bIsTwoPlayerMode) Image_TwoM1->SetVisibility(ESlateVisibility::Visible);
    	else Image_OneM1->SetVisibility(ESlateVisibility::Visible);
        break;
        case 2:
        if (GI->bIsTwoPlayerMode) Image_TwoM2->SetVisibility(ESlateVisibility::Visible);
    	else Image_OneM2->SetVisibility(ESlateVisibility::Visible);
        break;
        case 3:
        if (GI->bIsTwoPlayerMode) Image_TwoM3->SetVisibility(ESlateVisibility::Visible);
    	else Image_OneM3->SetVisibility(ESlateVisibility::Visible);
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
        //UE_LOG(LogTemp, Log, TEXT("UMG:map选择界面:超过范围或初始化选择状态"));
        break;
    }
	PlayAnimation(Anim_ChoseShare);
}

void UChoseMapWidget::OnDelayLoadNewLevel()
{
	AMainMenuGameMode* MyGM = GetCustomGameMode();
	MyGM->StartGameLevel(MapName);//切换关卡
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "StartHUD.h"

#include "ChoseMapWidget.h"
#include "ChoseSkinWidget.h"
#include "SelsectPlayerWidget.h"
#include "Blueprint/UserWidget.h"
#include "StartUserWidget.h"
#include "GameInstance/MyGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"


void AStartHUD::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwningPlayerController()->SetShowMouseCursor(true);//显示鼠标
	//关闭玩家控制器的输入操作
	//GetOwningPlayerController()->SetInputMode(FInputModeUIOnly());

	//循环播放BGM
	PlayLoopBGM();
	
	// 游戏启动时，根据 GameInstance 枚举加载对应界面
	LoadWidgetByGameInstanceEnum();
}

void AStartHUD::CreateStartMenu()
{
	if (!StartWidget)
	{
		TSubclassOf<UStartUserWidget> WidgetClass = LoadClass<UStartUserWidget>(
			nullptr, 
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_Start.U_Start_C'")
		);
		if (WidgetClass)
		{
			StartWidget = CreateWidget<UStartUserWidget>(GetOwningPlayerController(), WidgetClass);
			if (StartWidget)
			{
				StartWidget->AddToViewport(); // 显示到屏幕
			}
		}
	}
}

void AStartHUD::LoadWidgetByGameInstanceEnum()
{
	// 第一步：获取 MyGameInstance 实例
	UMyGameInstance* MyGI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (!MyGI)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartHUD: 未能获取到 MyGameInstance！"));
		return;
	}

	// 第二步：隐藏所有旧界面，避免重叠
	//HideAllWidgets();

	// 第三步：根据枚举值 switch 加载对应 Widget
	switch (MyGI->G_WidgetChose)
	{
	case EG_Widget::Start:
		// 加载开始界面
		if (!StartWidget) // 先判断是否有已经实例化的,如果没有就创建并显示到屏幕上
		{
			CreateStartMenu();
		}
		else StartWidget->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("StartHUD: 加载 Start 界面"));
		break;
		
	case EG_Widget::ChosePlayer:
		if (!ChosePlayerWidget)
		{
			//先加载这个蓝图控件的引用路径
			TSubclassOf<USelsectPlayerWidget> WidgetClass = LoadClass<USelsectPlayerWidget>(
				nullptr, 
				TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_SelectPlayer.U_SelectPlayer_C'"));
			if (WidgetClass)//如果能成功加载蓝图控件
			{
				//创建实例
				ChosePlayerWidget = CreateWidget<USelsectPlayerWidget>(GetOwningPlayerController(), WidgetClass);
				if (ChosePlayerWidget)
				{
					ChosePlayerWidget->AddToViewport(); // 显示到屏幕
				}
			}
		}
		else ChosePlayerWidget->AddToViewport();
		UE_LOG(LogTemp, Log, TEXT("StartHUD: 加载 ChosePlayer 界面"));
		break;	
		

	case EG_Widget::ChoseSkin:
		if (!ChoseSkinWidget)
		{
			TSubclassOf<UChoseSkinWidget> WidgetClass = LoadClass<UChoseSkinWidget>
			(
			nullptr, 
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_ChoseSkin.U_ChoseSkin_C'")
			);
			if (WidgetClass)//如果能成功加载蓝图控件
			{
				//创建实例
				ChoseSkinWidget = CreateWidget<UChoseSkinWidget>(GetOwningPlayerController(), WidgetClass);
				if (ChoseSkinWidget)
				{
					ChoseSkinWidget->AddToViewport(); // 显示到屏幕
				}
			}			
			else ChoseSkinWidget->AddToViewport();
		}
		UE_LOG(LogTemp, Log, TEXT("StartHUD: 加载 ChoseSkin 界面"));
		break;
	
	case EG_Widget::ChoseMap:
		if (!ChoseMapWidget)
		{
			TSubclassOf<UChoseMapWidget> WidgetClass = LoadClass<UChoseMapWidget>
			(
			nullptr, 
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_ChoseMap.U_ChoseMap_C'")
			);
			if (WidgetClass)//如果能成功加载蓝图控件
			{
				//创建实例
				ChoseMapWidget = CreateWidget<UChoseMapWidget>(GetOwningPlayerController(), WidgetClass);
				if (ChoseMapWidget)
				{
					ChoseMapWidget->AddToViewport(); // 显示到屏幕
				}
			}			
			else ChoseMapWidget->AddToViewport();
		}
		UE_LOG(LogTemp, Log, TEXT("StartHUD: 加载 ChoseMap 界面"));
		break;

	case  EG_Widget::ExitGame:
		UE_LOG(LogTemp, Log, TEXT("ExitGame界面还没写"));
		break;
		
	default:
		// 默认加载 Start 界面
		CreateStartMenu();
		UE_LOG(LogTemp, Warning, TEXT("StartHUD: 未知枚举值，默认加载 Start 界面"));
		break;	
	}
	
	
	
	
	
}

void AStartHUD::PlayLoopBGM()
{
	OnBgmPlayCallback();
	
	// 设置循环定时器
	const float BgmTotalDuration = 73.0f; 
	GetWorldTimerManager().SetTimer(
	BgmLoopTimerHandle, // 传递声明好的句柄（引用类型，语法合法）
	this, // 所有者指针（HUD 实例）
	&AStartHUD::OnBgmPlayCallback, // 回调函数（UE 推荐的成员函数指针格式）
	BgmTotalDuration, // 定时时长
	true // 开启循环（定时器自动重复，无需重新创建）
);
}

// 新增：BGM 播放回调（仅负责播放 BGM，被定时器循环调用）
void AStartHUD::OnBgmPlayCallback()
{
	TObjectPtr<UMyGameInstance> GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->LoadAndPlaySound2D(BgmPath, 0.5f);
}
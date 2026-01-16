// Fill out your copyright notice in the Description page of Project Settings.


#include "StartHUD.h"
#include "Blueprint/UserWidget.h"
#include "StartUserWidget.h"
#include "GameInstance/MyGameInstance.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


void AStartHUD::BeginPlay()
{
	Super::BeginPlay();
	
	GetOwningPlayerController()->SetShowMouseCursor(true);//显示鼠标
	//关闭玩家控制器的输入操作
	//GetOwningPlayerController()->SetInputMode(FInputModeUIOnly());
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

void AStartHUD::HideAllWidgets()
{
	// 把所有管理的Widget放入数组，批量移除
	TArray<UUserWidget*> ManagedWidgets = {
		StartWidget,

	};

	// 遍历移除，逻辑简洁且无依赖
	for (UUserWidget* Widget : ManagedWidgets)
	{
		if (Widget != nullptr) // 简化判断：只要Widget存在，就尝试移除
		{
			Widget->RemoveFromViewport();
		}
	}
	UE_LOG(LogTemp, Log, TEXT("StartHUD: 已隐藏所有管理的Widget"));
	
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
	HideAllWidgets();

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

	case EG_Widget::ChoseSkin://还没写
		UE_LOG(LogTemp, Log, TEXT("ChoseSkin界面还没写"));
		break;
	
	case EG_Widget::ChoseMap://还没写
		UE_LOG(LogTemp, Log, TEXT("ChoseMap界面还没写"));
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
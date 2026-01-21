// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoseMapWidget.h"
#include "SelsectPlayerWidget.h"
#include "Components/Button.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Mouth04/MainMenuGameMode.h"


void UChoseMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (Button_InMap)
	{
		Button_InMap->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedInMap);
	}
	if (Button_ChosePlayer)
	{
		Button_ChosePlayer->OnClicked.AddDynamic(this, &UChoseMapWidget::OnClickedChosePlayer);
	}
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void UChoseMapWidget::OnClickedInMap()
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
	GI->SetMaxBox("Coop_01");//设置最大盒子数
	MyGM->StartGameLevel("Coop_01");
	
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
	GameInstance->G_WidgetChose = EG_Widget::ChosePlayer;//标记进入游戏选择界面
	this->RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("UMG: chose Map 已从视口移除！"));
}

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



// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Players/ABoxBot.h"
#include "Public/GameInstance/MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Players/MyPlayerController.h"
#include "UI/StartHUD.h"



void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (PC)
	{
		PC->bShowMouseCursor = true;
		
		FInputModeUIOnly InputModeData;
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		
		PC->SetInputMode(InputModeData);
	}
	if (MainMenuWidgetClass)
	{
		UUserWidget* MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Error: Please assign MainMenuWidgetClass in Blueprint!"));
	}
}

AMainMenuGameMode::AMainMenuGameMode()
{
	DefaultPawnClass = nullptr;
	//设置默认的HUD
	static ConstructorHelpers::FClassFinder<AStartHUD> MyHUDPath(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/UMG/BP_StartHUD.BP_StartHUD_C'"));
	if(MyHUDPath.Class)
	{
		HUDClass= MyHUDPath.Class;
	}
	PlayerControllerClass=AMyPlayerController::StaticClass();
	
	
}

void AMainMenuGameMode::SetTwoPlayerMode(bool bEnable)
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI)
	{
		GI->bIsTwoPlayerMode = bEnable;
		
	}
}

void AMainMenuGameMode::ConfirmPlayer1Selection(TSubclassOf<AABoxBot> SelectedClass, EGlobalPlayerType PlayerType)
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI && SelectedClass)
	{
		GI->G_P1SelectedClass = SelectedClass;
		GI->G_P1PlayerType = PlayerType;
		
	
	}
}

void AMainMenuGameMode::ConfirmPlayer2Selection(TSubclassOf<AABoxBot> SelectedClass, EGlobalPlayerType PlayerType)
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI && SelectedClass)
	{
		GI->G_P2SelectedClass = SelectedClass;
		GI->G_P2PlayerType = PlayerType;
		FString ClassName = SelectedClass->GetName();
	}
}

void AMainMenuGameMode::StartGameLevel(FName LevelName)
{
	if (LevelName.IsNone()) return;
	
	
	// 切换关卡
	UGameplayStatics::OpenLevel(this, LevelName);
}

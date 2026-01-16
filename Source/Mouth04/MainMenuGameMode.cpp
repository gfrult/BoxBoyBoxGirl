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
		FString Msg = bEnable ? TEXT("Mode: 2 Players") : TEXT("Mode: 1 Player");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Msg);
	}
}

void AMainMenuGameMode::ConfirmPlayer1Selection(TSubclassOf<AABoxBot> SelectedClass)
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI && SelectedClass)
	{
		GI->G_P1SelectedClass = SelectedClass;
		
		FString ClassName = SelectedClass->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("P1 Selected: ") + ClassName);
	}
}

void AMainMenuGameMode::ConfirmPlayer2Selection(TSubclassOf<AABoxBot> SelectedClass)
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
	if (GI && SelectedClass)
	{
		GI->G_P2SelectedClass = SelectedClass;
		
		FString ClassName = SelectedClass->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("P2 Selected: ") + ClassName);
	}
}

void AMainMenuGameMode::StartGameLevel(FName LevelName)
{
	if (LevelName.IsNone()) return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Loading Level..."));
	
	// 切换关卡
	UGameplayStatics::OpenLevel(this, LevelName);
}

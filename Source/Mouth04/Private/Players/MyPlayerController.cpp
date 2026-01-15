// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/MyPlayerController.h"
#include "Players/ABoxBot.h"             
#include "Kismet/GameplayStatics.h"      
#include "EnhancedInputSubsystems.h"     
#include "EnhancedInputComponent.h"      
#include "Components/InputComponent.h"



void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyPlayerController::DelayedInit, 0.2f, false);
}
void AMyPlayerController::DelayedInit()
{
	CachedID = UGameplayStatics::GetPlayerControllerID(this);
	bIsCoopMode = (GetWorld()->GetNumPlayerControllers() > 1);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if (Subsystem)
	{
		Subsystem->ClearAllMappings();
		
		int32 MyID= UGameplayStatics::GetPlayerControllerID(this);
		int32 TotalPlayers = GetWorld()->GetNumPlayerControllers();
		UE_LOG(LogTemp, Warning, TEXT(">>> [%d号] "), MyID);
		UE_LOG(LogTemp, Warning, TEXT(">>> 有%d个 "), TotalPlayers);
		if (TotalPlayers <= 1)
		{
			if (IMC_Single)
			{
				Subsystem->AddMappingContext(IMC_Single,0);
				UE_LOG(LogTemp, Warning, TEXT(">>> [%d号] 进入单人模式逻辑"), MyID);
			}
		}
		if (TotalPlayers >1)
		{
			if (IMC_Double)
			{
				Subsystem->AddMappingContext(IMC_Double,0);
				UE_LOG(LogTemp, Warning, TEXT(">>> [%d号] 进入双人模式逻辑,IMC_P1_Coop"), MyID);
			}
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (IA_Move)
		{
			EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AMyPlayerController::OnMove_Enhanced);
			EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Completed, this, &AMyPlayerController::OnMoveEnd_Enhanced);
		}
		if (IA_Jump)
			EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &AMyPlayerController::OnJump_Enhanced);
		
		if (IA_Spawn)
		{
			EnhancedInputComponent->BindAction(IA_Spawn, ETriggerEvent::Started, this, &AMyPlayerController::OnSpawnStart_Enhanced);
			EnhancedInputComponent->BindAction(IA_Spawn, ETriggerEvent::Completed, this, &AMyPlayerController::OnSpawnEnd_Enhanced);
		}
		
		if (IA_SpawnUp)
			EnhancedInputComponent->BindAction(IA_SpawnUp, ETriggerEvent::Started, this, &AMyPlayerController::OnSpawnUp_Enhanced);
		
		if (IA_PutDown)
		{
			EnhancedInputComponent->BindAction(IA_PutDown, ETriggerEvent::Started, this, &AMyPlayerController::OnPutDownStart_Enhanced);
			EnhancedInputComponent->BindAction(IA_PutDown, ETriggerEvent::Completed, this, &AMyPlayerController::OnPutDownEnd_Enhanced);
		}
		
		if (IA_RemoveBox)
			EnhancedInputComponent->BindAction(IA_RemoveBox, ETriggerEvent::Started, this, &AMyPlayerController::OnRemoveBox_Enhanced);
		
		if (IA_MoveP2)
		{
			EnhancedInputComponent->BindAction(IA_MoveP2, ETriggerEvent::Triggered, this, &AMyPlayerController::OnMove_EnhancedP2);
			EnhancedInputComponent->BindAction(IA_MoveP2, ETriggerEvent::Completed, this, &AMyPlayerController::OnMoveEnd_EnhancedP2);
		}
		if (IA_JumpP2)
			EnhancedInputComponent->BindAction(IA_JumpP2, ETriggerEvent::Started, this, &AMyPlayerController::OnJump_EnhancedP2);
		
		if (IA_SpawnP2)
		{
			EnhancedInputComponent->BindAction(IA_SpawnP2, ETriggerEvent::Started, this, &AMyPlayerController::OnSpawnStart_EnhancedP2);
			EnhancedInputComponent->BindAction(IA_SpawnP2, ETriggerEvent::Completed, this, &AMyPlayerController::OnSpawnEnd_EnhancedP2);
		}
		
		if (IA_SpawnUpP2)
			EnhancedInputComponent->BindAction(IA_SpawnUpP2, ETriggerEvent::Started, this, &AMyPlayerController::OnSpawnUp_EnhancedP2);
		
		if (IA_PutDownP2)
		{
			EnhancedInputComponent->BindAction(IA_PutDownP2, ETriggerEvent::Started, this, &AMyPlayerController::OnPutDownStart_EnhancedP2);
			EnhancedInputComponent->BindAction(IA_PutDownP2, ETriggerEvent::Completed, this, &AMyPlayerController::OnPutDownEnd_EnhancedP2);
		}
		
		if (IA_RemoveBoxP2)
			EnhancedInputComponent->BindAction(IA_RemoveBoxP2, ETriggerEvent::Started, this, &AMyPlayerController::OnRemoveBox_EnhancedP2);
	}
}

void AMyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AABoxBot* MyBot = Cast<AABoxBot>(GetPawn()))
	{
		MyBot->RightFunction(CurrentMoveInputP1); 
	}
	
	if (CachedID == 0 && bIsCoopMode)
	{
		if (AMyPlayerController* P2PC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 1)))
		{
			P2PC->CurrentMoveInputP1 = this->CurrentMoveInputP2;
		}
	}
}

void AMyPlayerController::OnMove_Enhanced(const FInputActionValue& Value)
{
	float MovementVector = Value.Get<float>();
	
	AABoxBot* ControlledBot = Cast<AABoxBot>(GetPawn());
	
	if (ControlledBot)
	{
		CurrentMoveInputP1=MovementVector;
	}
}

void AMyPlayerController::OnJump_Enhanced(const FInputActionValue& Value)
{
	if (Cast<AABoxBot>(GetPawn()))
	{
		Cast<AABoxBot>(GetPawn())->JumpFunction();
	}
}

void AMyPlayerController::OnSpawnStart_Enhanced(const FInputActionValue& Value)
{
	if (Cast<AABoxBot>(GetPawn()))
	{
		Cast<AABoxBot>(GetPawn())->BeginSpawnBox();
	}
}

void AMyPlayerController::OnSpawnEnd_Enhanced(const FInputActionValue& Value)
{
	if (Cast<AABoxBot>(GetPawn()))
	{
		Cast<AABoxBot>(GetPawn())->EndSpawnBox();
	}
}

void AMyPlayerController::OnSpawnUp_Enhanced(const FInputActionValue& Value)
{
	if (Cast<AABoxBot>(GetPawn()))
	{
		Cast<AABoxBot>(GetPawn())->OnSpawnUp();
	}
}

void AMyPlayerController::OnPutDownStart_Enhanced(const FInputActionValue& Value)
{
	if (Cast<AABoxBot>(GetPawn()))
	{
		Cast<AABoxBot>(GetPawn())->BeginPutDownBox();
	}
}

void AMyPlayerController::OnPutDownEnd_Enhanced(const FInputActionValue& Value)
{
	if (Cast<AABoxBot>(GetPawn()))
	{
		Cast<AABoxBot>(GetPawn())->EndPutDownBox();
	}
}

void AMyPlayerController::OnRemoveBox_Enhanced(const FInputActionValue& Value)
{
	if (Cast<AABoxBot>(GetPawn()))
	{
		Cast<AABoxBot>(GetPawn())->RemoveDroppedBoxes();
	}
}

void AMyPlayerController::OnMoveEnd_Enhanced(const FInputActionValue& Value)
{
	AABoxBot* ControlledBot = Cast<AABoxBot>(GetPawn());

	if (ControlledBot)
	{
		CurrentMoveInputP1=0;
	}
}

void AMyPlayerController::OnMove_EnhancedP2(const FInputActionValue& Value)
{
	
	APawn* P2Pawn = UGameplayStatics::GetPlayerPawn(this, 1);
	
	if (Cast<AABoxBot>(P2Pawn))
	{
		CurrentMoveInputP2=Value.Get<float>();
	}
}

void AMyPlayerController::OnJump_EnhancedP2(const FInputActionValue& Value)
{
	APawn* P2Pawn = UGameplayStatics::GetPlayerPawn(this, 1);
	
	if (AABoxBot* P2Bot = Cast<AABoxBot>(P2Pawn))
	{
		P2Bot->JumpFunction();
	}
}

void AMyPlayerController::OnSpawnStart_EnhancedP2(const FInputActionValue& Value)
{
	APawn* P2Pawn = UGameplayStatics::GetPlayerPawn(this, 1);
	
	if (AABoxBot* P2Bot = Cast<AABoxBot>(P2Pawn))
	{
		P2Bot->BeginSpawnBox();
	}
}

void AMyPlayerController::OnSpawnEnd_EnhancedP2(const FInputActionValue& Value)
{
	APawn* P2Pawn = UGameplayStatics::GetPlayerPawn(this, 1);
	
	if (AABoxBot* P2Bot = Cast<AABoxBot>(P2Pawn))
	{
		P2Bot->EndSpawnBox();
	}
}

void AMyPlayerController::OnSpawnUp_EnhancedP2(const FInputActionValue& Value)
{
	APawn* P2Pawn = UGameplayStatics::GetPlayerPawn(this, 1);
	
	if (AABoxBot* P2Bot = Cast<AABoxBot>(P2Pawn))
	{
		P2Bot->OnSpawnUp();
	}
}

void AMyPlayerController::OnPutDownStart_EnhancedP2(const FInputActionValue& Value)
{
	APawn* P2Pawn = UGameplayStatics::GetPlayerPawn(this, 1);
	
	if (AABoxBot* P2Bot = Cast<AABoxBot>(P2Pawn))
	{
		P2Bot->BeginPutDownBox();
	}
}

void AMyPlayerController::OnPutDownEnd_EnhancedP2(const FInputActionValue& Value)
{
	APawn* P2Pawn = UGameplayStatics::GetPlayerPawn(this, 1);
	
	if (AABoxBot* P2Bot = Cast<AABoxBot>(P2Pawn))
	{
		P2Bot->EndPutDownBox();
	}
}

void AMyPlayerController::OnRemoveBox_EnhancedP2(const FInputActionValue& Value)
{
	APawn* P2Pawn = UGameplayStatics::GetPlayerPawn(this, 1);
	
	if (AABoxBot* P2Bot = Cast<AABoxBot>(P2Pawn))
	{
		P2Bot->RemoveDroppedBoxes();
	}
}

void AMyPlayerController::OnMoveEnd_EnhancedP2(const FInputActionValue& Value)
{
	APawn* P2Pawn = UGameplayStatics::GetPlayerPawn(this, 1);
	
	if (AABoxBot* P2Bot = Cast<AABoxBot>(P2Pawn))
	{
		CurrentMoveInputP2=0;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InputActionValue.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;

UCLASS()
class MOUTH04_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "IMC")
	UInputMappingContext* IMC_Single;
	UPROPERTY(EditAnywhere, Category = "IMC")
	UInputMappingContext* IMC_Double;

	
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_PutDown;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_RemoveBox;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_Spawn;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_SpawnUp;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_JumpP2;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_MoveP2;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_PutDownP2;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_RemoveBoxP2;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_SpawnP2;
	UPROPERTY(EditAnywhere, Category = "IA")
	UInputAction* IA_SpawnUpP2;
	
	float CurrentMoveInputP1 = 0.0f;
	float CurrentMoveInputP2 = 0.0f;
	int32 CachedID = -1;
	bool bIsCoopMode = false;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	
	void DelayedInit();
	void OnMove_Enhanced(const FInputActionValue& Value);
	void OnJump_Enhanced(const FInputActionValue& Value);
	void OnSpawnStart_Enhanced(const FInputActionValue& Value);
	void OnSpawnEnd_Enhanced(const FInputActionValue& Value);
	void OnSpawnUp_Enhanced(const FInputActionValue& Value);
	void OnPutDownStart_Enhanced(const FInputActionValue& Value);
	void OnPutDownEnd_Enhanced(const FInputActionValue& Value);
	void OnRemoveBox_Enhanced(const FInputActionValue& Value);
	void OnMoveEnd_Enhanced(const FInputActionValue& Value);
	
	void OnMove_EnhancedP2(const FInputActionValue& Value);
	void OnJump_EnhancedP2(const FInputActionValue& Value);
	void OnSpawnStart_EnhancedP2(const FInputActionValue& Value);
	void OnSpawnEnd_EnhancedP2(const FInputActionValue& Value);
	void OnSpawnUp_EnhancedP2(const FInputActionValue& Value);
	void OnPutDownStart_EnhancedP2(const FInputActionValue& Value);
	void OnPutDownEnd_EnhancedP2(const FInputActionValue& Value);
	void OnRemoveBox_EnhancedP2(const FInputActionValue& Value);
	void OnMoveEnd_EnhancedP2(const FInputActionValue& Value);
	
};

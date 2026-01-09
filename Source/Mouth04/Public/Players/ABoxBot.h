// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ABoxBot.generated.h"

UCLASS()
class MOUTH04_API AABoxBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AABoxBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;
	class UPaperFlipbookComponent* FootFlipbookComponent;
	class UBoxComponent* BoxBody;
	class USphereComponent* BoxFoot;
	class UPhysicalMaterial* BotPhysMat;
	TArray<class AActor*> BoxChain;
	class UPaperSpriteComponent* BodySpriteComponent;
	class UPaperFlipbookComponent* EyesFlipbookComponent;
	class UPaperFlipbook* FootPaperFlipbook;
	class UPaperFlipbook* StandPaperFlipbook;
	class UPaperFlipbook* JumpPaperFlipbook;
	
	void RightFunction(float AxisValue);
	void JumpFunction();
	void BeginSpawnBox();
	void EndSpawnBox();
	bool CheckIsOnGround();
	void SpawnBox(FVector Direction);
	void OnSpawnUp();
	void OnSpawnRight();
	void OnSpawnLeft();
	void OnSpawnDown();
	void BeginPutDownBox();
	void EndPutDownBox();
	bool CheckIsHooked();
	
	bool IsSpawnMode;
	bool ClockSpawnLeft;
	bool ClockSpawnRight;
	bool IsInAir;
	bool IsPutDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxBoy Settings", meta = (ClampMin = "0"))
	int32 MaxBoxNumber=5;
	
};

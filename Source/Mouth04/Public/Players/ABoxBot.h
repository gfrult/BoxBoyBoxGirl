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
	TArray<AActor*> BoxChain;
	TArray<AActor*> DroppedBoxes;
	class UPaperSpriteComponent* BodySpriteComponent;
	class UPaperFlipbookComponent* EyesFlipbookComponent;
	class UPaperFlipbook* RunPaperFlipbook;
	class UPaperFlipbook* StandPaperFlipbook;
	class UPaperFlipbook* HookPaperFlipbook;
	class UPaperFlipbook* JumpPaperFlipbook;
	class UPaperSprite* BoxSheepB;
	class UPaperFlipbook* SpawnBoxEyesFlipbook;
	class UPaperFlipbook* EyesFlipbook;
	
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
	void PutDownBox();
	void RemoveDroppedBoxes();
	void ThrowBox(float ThrowVector);
	
	bool bIsSpawnMode;
	bool bClockSpawnLeft;
	bool bClockSpawnRight;
	bool bIsInAir;
	bool bIsPutDown;
	bool bIsThrowing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxBoy Settings", meta = (ClampMin = "0"))
	int32 MaxBoxNumber=5;
	float ShakeAmplitude = 1.4f;	// 颤动的偏移幅度（厘米）
	float ShakeFrequency = 9.0f;	// 颤动的频率（每秒震动次数）
	FVector BodySpriteInitialRelativeLoc;	// 记录初始相对位置（确保复位准确）
	float PlayerXVector;//玩家朝向
	float BoxYVector;//第一个方块相对玩家的位置
	
};

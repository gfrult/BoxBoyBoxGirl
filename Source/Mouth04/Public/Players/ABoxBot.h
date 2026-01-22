// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ABoxBot.generated.h"

#define COLLISION_P1 ECC_GameTraceChannel1 
#define COLLISION_P2 ECC_GameTraceChannel2

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
	virtual void PossessedBy(AController* NewController) override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;
	
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
	class USphereComponent* Wheel1;
	class USphereComponent* Wheel2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Assets | Visuals")
	class UPaperSprite* BodySprite;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Assets | Visuals")
	class UPaperSprite* BoxA;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Assets | Visuals")
	class UPaperSprite* BoxB;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Assets | Visuals")
	class UPaperFlipbook* SpawnBoxEyesFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Assets | Visuals")
	class UPaperFlipbook* EyesFlipbook;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Assets | Visuals")
	class UTexture2D* PlayerType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Assets | Visuals")
	class UTexture2D* BoxType;
	
	class UPaperFlipbook* RunPaperFlipbook;
	
	class UPaperFlipbook* StandPaperFlipbook;
	
	class UPaperFlipbook* HookPaperFlipbook;
	
	class UPaperFlipbook* JumpPaperFlipbook;
	
	class UPaperFlipbook* SquatPaperFlipbook;//下蹲
	
	class UPaperSprite* UpArrow;
	
	class UPaperSpriteComponent* UpArrowCom;
	
	class AGoalActor* GoalDoor=nullptr;
	
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
	void SwitchToJumpFlipbook();// 切换跳跃动画
	void SwitchToStandFlipbook();// 切换站立动画
	void CollectJumpTeam(AActor* CurrentActor,TArray<AActor*>& OutTeam, TSet<AActor*>& Visited);//扫描所有物体，用于跳跃时抬升身上所有物体
	bool CanTeamJump(const TArray<AActor*>& TeamMembers);
	void ExecuteTeamJump(const TArray<AActor*>& TeamMembers);
	void MoveUp(const TArray<AActor*>& TeamMembers);
	void TryStandUp();
	void UploadtoGameInstance();
	void Respawn();
	UFUNCTION()
	void  OnSpikeHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	bool bIsSpawnMode;
	bool bClockSpawnLeft;
	bool bClockSpawnRight;
	bool bIsInAir;
	bool bIsPutDown;
	bool bIsThrowing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BoxBoy Settings", meta = (ClampMin = "0"))
	int32 MaxBoxNumber=5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BoxBoy Settings")
	int32 RemainingBoxNumber;
	float ShakeAmplitude = 1.4f;	// 颤动的偏移幅度（厘米）
	float ShakeFrequency = 9.0f;	// 颤动的频率（每秒震动次数）
	FVector BodySpriteInitialRelativeLoc;	// 记录初始相对位置（确保复位准确）
	float PlayerXVector;//玩家朝向
	float BoxYVector;//第一个方块相对玩家的位置
	int8 MyID;
	FVector RespawnLocation;
	int32 PlayerStarNum=0;
	
private:
	UPROPERTY()
	FTimerHandle SquatTimerHandle; // 下蹲动画的计时器
	bool bIsPlayingSquat = false; // 是否正在播放下蹲动画
	bool bWasOnGround = false;
	
};

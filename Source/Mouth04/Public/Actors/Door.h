// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class MOUTH04_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	class UPaperSpriteComponent* DoorSpriteComponent;
	
	UPROPERTY(EditAnywhere,Category="Door Sprite")
	class UPaperSprite* DoorSprite;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* DoorComponent;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* SafetyBox;
	
	UFUNCTION(BlueprintCallable)
	void OpenDoor();
	UFUNCTION(BlueprintCallable)
	void CloseDoor();
	
	FVector MoveOffset = FVector(0.f, 0.f, 320.f);
	float MoveSpeed = 5.f;
	
	FVector InitialLocation; 
	FVector TargetLocation;  
	bool bIsOpen = false;
	
	UPROPERTY(EditAnywhere,Category="Buttons")
	TArray<class AActionButton*> DoorButtons;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SharedCamera.generated.h"

UCLASS()
class MOUTH04_API ASharedCamera : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASharedCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	class UCameraComponent* CameraComp;
	class USpringArmComponent* SpringArm;
	class USceneComponent* RootScene;
	
	float MinArmLength = 4500.0f;
	float MaxArmLength = 9000.0f;
	float ZoomMultiplier = 2.2f;
	float ZoomInterpSpeed = 2.0f;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxActor.generated.h"

UCLASS()
class MOUTH04_API ABoxActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABoxActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	class UPaperSpriteComponent* SpriteComponent;
	class UBoxComponent* Box;
	class UPhysicalMaterial* BotPhysMat;
	class USphereComponent* Wheel;//略微垫高保持平稳
};

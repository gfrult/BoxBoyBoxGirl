// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActionButton.generated.h"

UCLASS()
class MOUTH04_API AActionButton : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AActionButton();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere,Category = "Box Settings")
	class UBoxComponent* ButtonBox;
	UPROPERTY(EditAnywhere,Category = "Box Settings")
	class UBoxComponent* BaseBox;
	

	class UPaperSpriteComponent* ButtonPaperSpriteComponent;
	UPROPERTY(EditAnywhere,Category = "Sprite Settings")
	class UPaperSprite* ButtonUnPressed;
	UPROPERTY(EditAnywhere,Category = "Sprite Settings")
	class UPaperSprite* ButtonPressed;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	void CheckOverlap();
	
	bool bIsPressed = false;
	bool bIsProcessing=false;
};

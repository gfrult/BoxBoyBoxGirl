// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindArea.generated.h"

UCLASS()
class MOUTH04_API AWindArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWindArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* WindBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind Settings")
	float WindStrength = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind Settings")
	float BlowDuration = 3.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind Settings")
	float StopDuration = 2.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wind Settings")
	bool bIsWindActive;
	
	UPROPERTY()
	TSet<class AABoxBot*> OverlappingBots;
	UPROPERTY()
	TSet<class ABoxActor*> OverlappingBoxes;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* SnowParticleComponent;
private:
	FTimerHandle WindTimerHandle;
	
	void ToggleWind();
};
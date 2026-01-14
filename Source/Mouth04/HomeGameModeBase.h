// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HomeGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MOUTH04_API AHomeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	AHomeGameModeBase();
	UPROPERTY(EditDefaultsOnly, Category = "Player Setup")
	TSubclassOf<class AABoxBot> Player1Class;

	UPROPERTY(EditDefaultsOnly, Category = "Player Setup")
	TSubclassOf<class AABoxBot> Player2Class;
};

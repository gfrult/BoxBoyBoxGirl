// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MOUTH04_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	public:
	
	// ========== 全局共享变量 ==========
	//P1
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	FString G_P1Name = TEXT("Sheep");//默认P1是小羊
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P1MaxBoxNumber = 0;
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P1RemainingBoxNumber = 0;
	
	//P2
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	FString G_P2Name = TEXT("None");//默认P2是None,即单人模式
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P2MaxBoxNumber = 0;
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P2RemainingBoxNumber = 0;

	
	
};

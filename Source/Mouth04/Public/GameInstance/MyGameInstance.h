// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

UENUM(BlueprintType) 
enum class EGlobalPlayerType : uint8
{
	None,       
	Sheep,     
	Pig,     
	Cow    
};

UCLASS()
class MOUTH04_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	public:
	
	// ========== 全局共享变量 ==========
	//P1
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	EGlobalPlayerType G_P1PlayerType = EGlobalPlayerType::Sheep;//在选择皮肤阶段进行 赋值
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P1MaxBoxNumber = 0;//在选择关卡阶段进行赋值
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P1RemainingBoxNumber = 0;//由进入游戏后玩家的实时数据更新赋值
	TSubclassOf<class AABoxBot> G_P1SelectedClass;//P1类型，G_P1SelectedClass.GetDefaultObject()->变量名
	
	//P2
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	EGlobalPlayerType G_P2PlayerType = EGlobalPlayerType::None;
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P2MaxBoxNumber = 0;
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P2RemainingBoxNumber = 0;
	TSubclassOf<class AABoxBot> G_P2SelectedClass;//P2类型
	


	
};

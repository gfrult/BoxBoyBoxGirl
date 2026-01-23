// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstance/MyGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Players/ABoxBot.h"


void UMyGameInstance::SetP1RemainingBoxNumber(int32 NewNumber)
{
	int32 ValidNumber = FMath::Max(NewNumber, 0);
	if (G_P1RemainingBoxNumber != ValidNumber)
	{
		G_P1RemainingBoxNumber = ValidNumber;
		// 正确访问类成员：this-> 可选，但加了更清晰
		this->OnP1RemainingBoxNumberChanged.Broadcast(ValidNumber);
		UE_LOG(LogTemp, Log, TEXT("GameInstance: 剩余箱子数更新为[%d]"), ValidNumber);
	}
}


int32 UMyGameInstance::GetP1RemainingBoxNumber() const
{
	return G_P1RemainingBoxNumber;
}


void UMyGameInstance::SetP2RemainingBoxNumber(int32 NewNumber)
{
	int32 ValidNumber = FMath::Max(NewNumber, 0);
	if (G_P2RemainingBoxNumber != ValidNumber)
	{
		G_P2RemainingBoxNumber = ValidNumber;
		this->OnP2RemainingBoxNumberChanged.Broadcast(ValidNumber);
		UE_LOG(LogTemp, Log, TEXT("GameInstance: P2剩余箱子数更新为[%d]"), ValidNumber);
	}
}


int32 UMyGameInstance::GetP2RemainingBoxNumber() const
{
	return G_P2RemainingBoxNumber;
}

FLevelConfig UMyGameInstance::GetLevelConfig(FName RowName)
{
	if (LevelConfigTable)
	{
		static const FString ContextString(TEXT("Level Config Lookup"));
		FLevelConfig* Row = LevelConfigTable->FindRow<FLevelConfig>(RowName, ContextString);
		if (Row) return *Row;
	}
	return FLevelConfig();
}

void UMyGameInstance::SetMaxBox(FName LevelName)
{
	FLevelConfig Config = GetLevelConfig(LevelName);
	G_P1MaxBoxNumber = Config.P1MaxBoxes;
	G_P2MaxBoxNumber = Config.P2MaxBoxes;
}

void UMyGameInstance::UpdateLevelProgress(FName LevelRowName, int32 StarsNum)
{
	// 查找记录
	FLevelData* Data = LevelProgressMap.Find(LevelRowName);

	if (Data)
	{
		// 标记通关
		Data->bCleared = true; 
		Data->UnlockStatus = ELevelStatus::Unlocked;

		// 更新星星 
		
		Data->StarNum = FMath::Max(Data->StarNum, StarsNum);
		
	}
	else
	{
		//如果是第一次玩
		FLevelData NewData;
		NewData.UnlockStatus = ELevelStatus::Unlocked;
		NewData.bCleared = true;
		NewData.StarNum = StarsNum; 

		LevelProgressMap.Add(LevelRowName, NewData);//存入Map
		
	}
	FLevelConfig CurrentConfig = GetLevelConfig(LevelRowName);
	
	if (!CurrentConfig.NextLevelName.IsNone())
	{
		UnlockLevel(CurrentConfig.NextLevelName);
	}
	
}

void UMyGameInstance::UnlockLevel(FName LevelRowName)
{
	if (LevelRowName.IsNone())return;
	FLevelData* Data = LevelProgressMap.Find(LevelRowName);
	if (Data)
	{
		if (Data->UnlockStatus == ELevelStatus::Locked)
		{
			Data->UnlockStatus = ELevelStatus::FirstUnlocked;
		}
	}
	else
	{
		FLevelData NewData;
		NewData.UnlockStatus = ELevelStatus::FirstUnlocked;
		LevelProgressMap.Add(LevelRowName, NewData);
	}
}

ELevelStatus UMyGameInstance::GetLevelStatus(FName LevelRowName)
{
	if (LevelRowName == "Solo_01" || LevelRowName == "Coop_01")
	{
		return ELevelStatus::Unlocked;
	}
	FLevelData* Data = LevelProgressMap.Find(LevelRowName);
	
	if (Data)
	{
		return Data->UnlockStatus;
	}
	return ELevelStatus::Locked;
}

int32 UMyGameInstance::GetStarNum(FName LevelRowName)
{
	FLevelData* Data = LevelProgressMap.Find(LevelRowName);
	if (Data)
	{
		return Data->StarNum;
	}
	return 0;
}

void UMyGameInstance::MarkLevelAsSeen(FName LevelRowName)
{
	FLevelData* Data = LevelProgressMap.Find(LevelRowName);
	if (Data)
	{
		Data->UnlockStatus = ELevelStatus::Unlocked;
	}
}

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_ConfigObj(TEXT("/Script/Engine.DataTable'/Game/Map/MapData/MapDataTable.MapDataTable'"));

	if (DT_ConfigObj.Object)
	{
		LevelConfigTable = DT_ConfigObj.Object;
	}
}

void UMyGameInstance::SetP1AnimalClass(TSubclassOf<AABoxBot> NewAnimalClass)
{
	G_P1SelectedClass = NewAnimalClass;
	if (G_P1SelectedClass)
	{
		UE_LOG(LogTemp, Log, TEXT("P1AnimalClass已切换为：%s"), *G_P1SelectedClass->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("P1AnimalClass被设置为无效类型！"));
	}
}

void UMyGameInstance::SetP2AnimalClass(TSubclassOf<AABoxBot> NewAnimalClass)
{
	G_P2SelectedClass = NewAnimalClass;
	if (G_P2SelectedClass)
	{
		UE_LOG(LogTemp, Log, TEXT("P2AnimalClass已切换为：%s"), *G_P2SelectedClass->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("P2AnimalClass被设置为无效类型！"));
	}
}
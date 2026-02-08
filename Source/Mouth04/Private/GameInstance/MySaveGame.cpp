// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MySaveGame.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


UMySaveGame::UMySaveGame()
{
	// 存档版本号：初始化为1，后续版本升级时可修改
	SaveGameVersion = 1;
	// 玩家数据数组：默认空，由GameInstance复制数据后赋值
	PlayerSaveDatas.Empty();
}

//静态工具函数：保存玩家数据到指定存档槽位
bool UMySaveGame::SavePlayerDatasToSlot(const TArray<FPlayerSaveData>& InPlayerDatas, const FString& SaveSlotName,
	int32 UserIndex)
{
	// 创建存档实例
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	if (!SaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[SaveGame IO] 保存失败：创建存档实例失败！"));
		return false;
	}
	// 赋值数据
	SaveGameInstance->PlayerSaveDatas = InPlayerDatas;
	SaveGameInstance->SaveGameVersion = 1;
	// 写入本地文件（纯IO）
	return UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, UserIndex);
}


//静态工具函数：从指定存档槽位加载玩家数据
bool UMySaveGame::LoadPlayerDatasFromSlot(TArray<FPlayerSaveData>& OutPlayerDatas, const FString& SaveSlotName,
	int32 UserIndex)
{
	// 检查存档是否存在
	if (!DoesSaveSlotExist(SaveSlotName, UserIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("[SaveGame IO] 加载失败：槽位%s无存档！"), *SaveSlotName);
		return false;
	}
	// 读取本地文件（纯IO）
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
	if (!SaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("[SaveGame IO] 加载失败：读取槽位%s失败！"), *SaveSlotName);
		return false;
	}
	// 输出数据
	OutPlayerDatas = SaveGameInstance->PlayerSaveDatas;
	return true;
}

//静态工具函数：检查指定槽位是否存在存档
bool UMySaveGame::DoesSaveSlotExist(const FString& SaveSlotName, int32 UserIndex)
{
	return UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex);
}

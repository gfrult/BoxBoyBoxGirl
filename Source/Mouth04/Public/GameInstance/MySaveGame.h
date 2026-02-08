// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */

struct FPlayerSaveData;
enum class ELevelStatus : uint8;
struct FLevelData;



UCLASS()
class MOUTH04_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UMySaveGame();

	/**
	 * 存档文件中存储的玩家数据数组
	 * 与GameInstance中的PlayerSaveDatas完全一致，用于本地持久化
	 * EditAnywhere：编辑器Details面板可编辑（便于测试）
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame|核心数据")
	TArray<FPlayerSaveData> PlayerSaveDatas;

	/**
	 * 存档版本号（用于后续游戏版本升级，兼容旧存档）
	 * 若后续修改了FPlayerSaveData结构，可通过版本号做数据兼容处理
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveGame|配置信息")
	int32 SaveGameVersion = 1;
	
	/**	
	* 静态工具函数：保存玩家数据到指定存档槽位
	* @param InPlayerDatas 要保存的玩家数据数组
	* @param SaveSlotName 存档槽位名（如"MainSaveSlot"）
	* @param UserIndex 用户ID（默认0）
	* @return bool 保存成功返回true
	*/
	UFUNCTION(BlueprintCallable, Category = "SaveGame|IO工具", meta=(DisplayName="保存玩家数据到槽位"))
	static bool SavePlayerDatasToSlot(const TArray<FPlayerSaveData>& InPlayerDatas, const FString& SaveSlotName = TEXT("MainSaveSlot"), int32 UserIndex = 0);

	/**
	 * 静态工具函数：从指定存档槽位加载玩家数据
	 * @param OutPlayerDatas 加载后的数据输出到该数组
	 * @param SaveSlotName 存档槽位名
	 * @param UserIndex 用户ID
	 * @return bool 加载成功返回true
	 */
	UFUNCTION(BlueprintCallable, Category = "SaveGame|IO工具", meta=(DisplayName="从槽位加载玩家数据"))
	static bool LoadPlayerDatasFromSlot(TArray<FPlayerSaveData>& OutPlayerDatas, const FString& SaveSlotName = TEXT("MainSaveSlot"), int32 UserIndex = 0);

	/**
	 * 静态工具函数：检查指定槽位是否存在存档
	 * @param SaveSlotName 存档槽位名
	 * @param UserIndex 用户ID
	 * @return bool 存在返回true
	 */
	UFUNCTION(BlueprintPure, Category = "SaveGame|IO工具", meta=(DisplayName="检查存档槽位是否存在"))
	static bool DoesSaveSlotExist(const FString& SaveSlotName = TEXT("MainSaveSlot"), int32 UserIndex = 0);
	

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "GameInstance/MyGameInstance.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Players/ABoxBot.h"
#include "GameInstance/MySaveGame.h"


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
	bIsRetractMode=Config.bCanRetract;
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
	
	// 存档实例指针初始化为空，避免野指针
	CurrentSaveGameInstance = nullptr;
	CurrentSelectedPlayerID = -1; // 初始未选择任何玩家
	LevelProgressMap.Empty();     // 初始清空实时关卡进度Map
}

void UMyGameInstance::Init()
{
	Super::Init();
	
	// 1. 从本地加载所有玩家存档数据
	bool bLoadSuccess = LoadPlayerDataFromLocal();
	if (bLoadSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("[游戏实例] 本地存档加载成功，共%d个玩家"), PlayerSaveDatas.Num());
		// 加载成功后，若有玩家则默认选中第一个玩家（同步其进度到LevelProgressMap）
		if (PlayerSaveDatas.Num() > 0)
		{
			SetCurrentSelectedPlayer(PlayerSaveDatas[0].PlayerID);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[游戏实例] 无本地存档，创建默认玩家"));
		// 无存档则创建默认玩家（自动设为当前选中，同步进度到LevelProgressMap）
		AddNewPlayer(TEXT("默认玩家"));
		// 保存默认玩家数据到本地
		SavePlayerDataToLocal();
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

// 全局通用 2D 音效播放函数（BlueprintCallable 支持全项目蓝图/代码调用）
void UMyGameInstance::LoadAndPlaySound2D(const FString& SoundPath, float Volume)
{
	// 步骤1：安全检查：音效路径不能为空
	if (SoundPath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("【全局音效】失败：音效路径为空！"));
		return;
	}

	// 步骤2：获取有效的世界上下文（GameInstance 中通过 GetWorld() 获取，全局可用）
	UWorld* CurrentWorld = GetWorld();

	// 步骤3：加载 SoundWave 音效资源（UE 自动缓存，重复加载不占用额外内存）
	USoundWave* TargetSound = LoadObject<USoundWave>(
		nullptr,
		*SoundPath,
		nullptr,
		LOAD_None
	);

	// 步骤4：安全检查 + 播放 2D 音效
	if (IsValid(TargetSound))
	{
		// 音量钳制：避免传入异常值导致音效问题（0.0f ~ 2.0f 区间）
		float ClampedVolume = FMath::Clamp(Volume, 0.0f, 2.0f);

		// 播放全局 2D 音效（无空间衰减，全场景可听）
		UGameplayStatics::PlaySound2D(
			CurrentWorld,
			TargetSound,
			ClampedVolume,
			1.0f, // 固定原始音调（已移除 Pitch 参数）
			0.0f
		);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("【音效】失败：加载音效资源失败！路径：%s"), *SoundPath);
	}
}


// 工具函数：生成唯一玩家ID（遍历所有玩家，取最大ID+1，保证不重复）
int32 UMyGameInstance::GenerateUniquePlayerID() const
{
	int32 MaxPlayerID = 0;
	for (const FPlayerSaveData& EachPlayer : PlayerSaveDatas)
	{
		if (EachPlayer.PlayerID > MaxPlayerID)
		{
			MaxPlayerID = EachPlayer.PlayerID;
		}
	}
	return MaxPlayerID + 1;
}

// 工具函数：根据PlayerID查找玩家在PlayerSaveDatas中的索引（未找到返回-1）
int32 UMyGameInstance::FindPlayerIndexByID(int32 PlayerID) const
{
	for (int32 i = 0; i < PlayerSaveDatas.Num(); i++)
	{
		if (PlayerSaveDatas[i].PlayerID == PlayerID && PlayerSaveDatas[i].bCanUse)
		{
			return i; // 找到有效玩家，返回索引
		}
	}
	UE_LOG(LogTemp, Error, TEXT("[玩家管理] 未找到玩家ID：%d"), PlayerID);
	return -1; // 未找到/玩家无效，返回-1
}

// 工具函数：同步当前LevelProgressMap到存档数组中对应玩家的PlayerLevelDatas中
bool UMyGameInstance::SyncLevelProgressToPlayerSaveData()
{
	// 1. 检查当前玩家ID是否有效
	if (CurrentSelectedPlayerID == -1)
	{
		UE_LOG(LogTemp, Warning, TEXT("[进度同步] 无当前选中玩家，无需同步进度"));
		return false;
	}
	// 2. 根据ID查找玩家索引
	int32 PlayerIndex = FindPlayerIndexByID(CurrentSelectedPlayerID);
	if (PlayerIndex == -1)
	{
		UE_LOG(LogTemp, Error, TEXT("[进度同步] 玩家ID%d不存在，同步失败"), CurrentSelectedPlayerID);
		return false;
	}
	// 3. 同步LevelProgressMap到该玩家的PlayerLevelDatas（直接赋值，覆盖旧存档进度）
	PlayerSaveDatas[PlayerIndex].PlayerLevelDatas = LevelProgressMap;
	UE_LOG(LogTemp, Warning, TEXT("[进度同步] 已将实时关卡进度同步到玩家ID%d的存档数据"), CurrentSelectedPlayerID);
	return true;
}

// 工具函数：将指定ID的玩家的存档进度同步到全局LevelProgressMap（清空旧数据，复制新数据）
bool UMyGameInstance::SyncPlayerSaveDataToLevelProgress(int32 PlayerID)
{
	// 1. 根据ID查找玩家索引
	int32 PlayerIndex = FindPlayerIndexByID(PlayerID);
	if (PlayerIndex == -1)
	{
		return false;
	}
	// 2. 清空旧的实时进度Map，避免数据残留
	LevelProgressMap.Empty();
	// 3. 复制玩家存档中的关卡数据到全局LevelProgressMap
	LevelProgressMap = PlayerSaveDatas[PlayerIndex].PlayerLevelDatas;
	UE_LOG(LogTemp, Warning, TEXT("[进度同步] 已将玩家ID%d的存档进度同步到实时LevelProgressMap，共%d个关卡"), PlayerID, LevelProgressMap.Num());
	return true;
}

// 保存玩家数据到本地（先同步当前玩家进度，再调用MySaveGame的IO工具整体保存）
void UMyGameInstance::SavePlayerDataToLocal()
{
	// 关键步骤：保存前先同步当前玩家的LevelProgressMap到存档数组（确保实时进度被保存）
	if (CurrentSelectedPlayerID != -1)
	{
		SyncLevelProgressToPlayerSaveData();
	}
	// 调用MySaveGame的静态IO函数，保存整个PlayerSaveDatas数组到本地
	bool bSaveSuccess = UMySaveGame::SavePlayerDatasToSlot(PlayerSaveDatas, TEXT("MainSaveSlot"), 0);
	if (bSaveSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("[存档管理] 保存成功！共%d个玩家，最后保存的是玩家ID%d"), PlayerSaveDatas.Num(), CurrentSelectedPlayerID);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[存档管理] 保存失败！"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("存档保存失败"));
	}
}

// 从本地加载玩家数据,并默认选择最后一个玩家数据
bool UMyGameInstance::LoadPlayerDataFromLocal()
{
	// 调用MySaveGame的静态IO函数，从本地加载整个PlayerSaveDatas数组
	bool bLoadSuccess = UMySaveGame::LoadPlayerDatasFromSlot(PlayerSaveDatas, TEXT("MainSaveSlot"), 0);
	if (bLoadSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("[存档管理] 加载成功！共%d个玩家"), PlayerSaveDatas.Num());
		// 加载成功后，若当前有选中的玩家ID，同步其进度到LevelProgressMap
		if (CurrentSelectedPlayerID != -1 && PlayerSaveDatas.Num() > 0)
		{
			// 若原选中的玩家ID不存在，则默认选中最后一个玩家
			if (FindPlayerIndexByID(CurrentSelectedPlayerID) == -1)
			{
				SetCurrentSelectedPlayer(PlayerSaveDatas.Last().PlayerID);
			}
			else
			{
				SyncPlayerSaveDataToLevelProgress(CurrentSelectedPlayerID);
			}
		}
		else if (CurrentSelectedPlayerID == -1 && PlayerSaveDatas.Num() > 0)
		{
			SetCurrentSelectedPlayer(PlayerSaveDatas.Last().PlayerID);
			UE_LOG(LogTemp, Log, TEXT("[存档管理] 首次加载，默认选中最后一个玩家，ID：%d"), PlayerSaveDatas.Last().PlayerID);
		}
	}
	return bLoadSuccess;
}

//存档
void UMyGameInstance::AutoSaveCurrentPlayer()
{
	if (PlayerSaveDatas.Num() > 0 && CurrentSelectedPlayerID != -1)
	{
		SyncLevelProgressToPlayerSaveData(); // 同步实时进度
		UMySaveGame::SavePlayerDatasToSlot(PlayerSaveDatas); // 保存到本地
		UE_LOG(LogTemp, Warning, TEXT("[自动保存] 已自动保存玩家ID%d的进度"), CurrentSelectedPlayerID);
	}
}

// 设置当前选中玩家（切换玩家时自动双向同步进度，保证数据一致）
bool UMyGameInstance::SetCurrentSelectedPlayer(int32 PlayerID)
{
	// 1. 检查要选中的玩家是否存在且有效
	if (FindPlayerIndexByID(PlayerID) == -1)
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(FString::Printf(TEXT("选中玩家失败：玩家ID%d不存在！"), PlayerID)));
		return false;
	}
	// 2. 若当前已有选中玩家，先同步其LevelProgressMap到存档数组（保存旧玩家的实时进度）
	if (CurrentSelectedPlayerID != -1)
	{
		SyncLevelProgressToPlayerSaveData();
	}
	// 3. 更新gameinstance中当前选中玩家ID
	CurrentSelectedPlayerID = PlayerID;
	// 4. 从本地读取玩家的存档进度到gameinstance的LevelProgressMap
	bool bSyncSuccess = SyncPlayerSaveDataToLevelProgress(PlayerID);
	if (bSyncSuccess)
	{
		FString PlayerName = PlayerSaveDatas[FindPlayerIndexByID(PlayerID)].PlayerName;
		//UE_LOG(LogTemp, Warning, TEXT("[玩家管理] 选中玩家成功：ID=%d，名称=%s"), PlayerID, *PlayerName);
	}
	return bSyncSuccess;
}

//新增玩家
int32 UMyGameInstance::AddNewPlayer(const FString& PlayerName)
{
	// 1. 初始化新玩家数据
	FPlayerSaveData NewPlayer;
	NewPlayer.PlayerID = GenerateUniquePlayerID(); // 生成唯一ID
	NewPlayer.PlayerName = PlayerName.IsEmpty() ? TEXT("无名玩家") : PlayerName;
	NewPlayer.bCanUse = true; // 标记为有效玩家
	// 2. 初始化新玩家的默认关卡进度
	TMap<FName, FLevelData> DefaultLevelDatas;
	DefaultLevelDatas.Add(FName("Coop_01"), FLevelData{ELevelStatus::FirstUnlocked, false, 0}); // 1关解锁
	DefaultLevelDatas.Add(FName("Solo_01"), FLevelData{ELevelStatus::FirstUnlocked, false, 0}); // 1关解锁
	NewPlayer.PlayerLevelDatas = DefaultLevelDatas;
	// 3. 将新玩家添加到全局存档数组
	PlayerSaveDatas.Add(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("[玩家管理] 新增玩家成功：ID=%d，名称=%s"), NewPlayer.PlayerID, *NewPlayer.PlayerName);
	// 4. 自动将新玩家设为当前选中玩家（同步其默认进度到LevelProgressMap）
	SetCurrentSelectedPlayer(NewPlayer.PlayerID);
	// 返回新玩家的唯一ID（而非索引，更稳定）
	return NewPlayer.PlayerID;
}



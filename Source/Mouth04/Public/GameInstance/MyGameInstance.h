#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"


class UStartUserWidget;
class UMySaveGame;
class AABoxBot;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnP1RemainingBoxNumberChanged, int32, value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnP2RemainingBoxNumberChanged, int32, value);

UENUM(BlueprintType) 
enum class EGlobalPlayerType : uint8
{
	None,       
	Sheep,     
	Pig,     
	Cow,
	Chick,
	Cat
};

UENUM(BlueprintType) 
enum class EG_Widget : uint8
{
	Start,//开始界面+单人模式or双人模式选择
	ChosePlayer,
	ChoseSkin,//皮肤选择界面
	ChoseMap,//关卡选择界面
	ExitGame//退出游戏界面
};

UENUM(BlueprintType)
enum class ELevelMode : uint8//关卡是单人关卡还是双人关卡
{
	Solo,
	Coop 
};

USTRUCT(BlueprintType)
struct FLevelConfig : public FTableRowBase//关卡配置
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELevelMode GameModeType = ELevelMode::Solo;//单人还是双人
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelMapName;//地图文件名
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName NextLevelName;//下一关
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 P1MaxBoxes = 5;//玩家1最大盒子数

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 P2MaxBoxes = 5;//玩家2最大盒子数
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRetract = false;//是否可以伸缩
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* GameBGM;//背景音乐
};

UENUM()
enum class ELevelStatus : uint8
{
	Locked=0,    //未解锁      
	FirstUnlocked=1,    //首次解锁
	Unlocked=2    //解锁
};

USTRUCT(BlueprintType)
struct FLevelData//关卡进度信息
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELevelStatus UnlockStatus = ELevelStatus::Locked; // 是否解锁

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCleared = false;  // 是否通关

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StarNum=0;    // 获得星星
	
};

USTRUCT(BlueprintType)
struct FPlayerSaveData
{
	GENERATED_BODY()
	FPlayerSaveData() = default;

	//玩家唯一ID（蓝图只读，由C++自动生成）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PlayerSave|基础信息")
	int32 PlayerID = 0;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSave|基础信息")
	FString PlayerName = TEXT("默认玩家");
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSave|基础信息")
	bool bCanUse = false;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSave|关卡数据")
	TMap<FName, FLevelData> PlayerLevelDatas;
};


UCLASS()
class MOUTH04_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	public:
	UMyGameInstance();
	// ========== 全局共享变量 ==========	
	
	UPROPERTY(BlueprintReadWrite, Category = "双人模式")
	bool bIsTwoPlayerMode;//是否选择双人模式	
	bool bIsRetractMode;
	UPROPERTY()
	EG_Widget G_WidgetChose = EG_Widget::Start;//标记需要加载的界面,初始化为start界面
	UPROPERTY()
	bool bIsShowSetting;
	UPROPERTY()
	UStartUserWidget* StartWidgetInstance;//存储开始界面实例指针

	//P1

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P1AnimalConfig")
	TSubclassOf<AABoxBot> G_P1SelectedClass; //存储AABoxBot类的子类型
	
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	EGlobalPlayerType G_P1PlayerType = EGlobalPlayerType::Sheep;
	
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P1MaxBoxNumber = 0;//在选择关卡阶段,通过读取关卡的数据 进行赋值
	//进入关卡生成玩家子类时,通过读取GameInstance上记录的数据来确定type 和 最大盒子数
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P1RemainingBoxNumber = 0;//由进入游戏后玩家的实时数据更新赋值

	
	//P2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P2AnimalConfig")
	TSubclassOf<AABoxBot> G_P2SelectedClass; //存储AABoxBot类的子类型
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	EGlobalPlayerType G_P2PlayerType = EGlobalPlayerType::Pig;
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P2MaxBoxNumber = 0;
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	int32 G_P2RemainingBoxNumber = 0;
	
	//C++接口，用来切换动物类型
	UFUNCTION(BlueprintCallable, Category = "AnimalConfig")
	void SetP1AnimalClass(TSubclassOf<AABoxBot> NewAnimalClass);
	UFUNCTION(BlueprintCallable, Category = "AnimalConfig")
	void SetP2AnimalClass(TSubclassOf<AABoxBot> NewAnimalClass);
	
	// 2. 委托变量声明（必须public，且名称和委托类型一致）
	UPROPERTY(BlueprintAssignable, Category = "GlobalData|Delegates")
	FOnP1RemainingBoxNumberChanged OnP1RemainingBoxNumberChanged;
	UPROPERTY(BlueprintAssignable, Category = "GlobalData|Delegates")
	FOnP2RemainingBoxNumberChanged OnP2RemainingBoxNumberChanged;
	
	
	// 封装函数：设置剩余箱子数并触发委托
	UFUNCTION(BlueprintCallable, Category = "GlobalData|BoxNumber")
	void SetP1RemainingBoxNumber(int32 NewNumber);
	UFUNCTION(BlueprintCallable, Category = "GlobalData|BoxNumber")
	void SetP2RemainingBoxNumber(int32 NewNumber);	
	
	
	// 封装函数：获取当前剩余箱子数
	UFUNCTION(BlueprintCallable, Category = "GlobalData|BoxNumber")
	int32 GetP1RemainingBoxNumber() const;
	UFUNCTION(BlueprintCallable, Category = "GlobalData|BoxNumber")
	int32 GetP2RemainingBoxNumber() const;
	
	//关卡数据表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "LevelConfig")
	class UDataTable* LevelConfigTable;

	// 辅助函数：查表
	UFUNCTION(BlueprintCallable, Category = "LevelConfig")
	FLevelConfig GetLevelConfig(FName RowName);
	
	//从表格读取最大盒子数并设置
	UFUNCTION(BlueprintCallable, Category = "LevelConfig")
	void SetMaxBox(FName LevelName);
	
	
	//关卡进度map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SaveData")
	TMap<FName, FLevelData> LevelProgressMap;
	
	//更新关卡进度
	UFUNCTION(BlueprintCallable, Category = "SaveData")
	void UpdateLevelProgress(FName LevelRowName, int32 StarsNum);

	//解锁某关 
	UFUNCTION(BlueprintCallable, Category = "SaveData")
	void UnlockLevel(FName LevelRowName);

	//查询某关是否解锁 
	UFUNCTION(BlueprintCallable, Category = "SaveData")
	ELevelStatus GetLevelStatus(FName LevelRowName);
	
	//获取关卡星星数
	UFUNCTION(BlueprintCallable, Category = "SaveData")
	int32 GetStarNum(FName LevelRowName);
	
	UFUNCTION(BlueprintCallable, Category = "SaveData")
	void MarkLevelAsSeen (FName LevelRowName);
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameInstance|存档数据|玩家列表")
    TArray<FPlayerSaveData> PlayerSaveDatas;//全局玩家存档数组（存储所有创建的玩家存档数据）
	
    UPROPERTY(BlueprintReadWrite, Category = "GameInstance|当前玩家|核心状态")
    int32 CurrentSelectedPlayerID = -1;// 当前选中的玩家索引（-1表示未选择任何玩家）

	// 保存当前所有玩家存档（先同步当前玩家LevelProgressMap到存档数组，再整体保存） 
	UFUNCTION(BlueprintCallable, Category = "GameInstance|存档管理|核心操作", meta=(DisplayName="保存玩家数据到本地"))
	void SavePlayerDataToLocal();

	// 从本地加载所有玩家存档（加载后自动同步当前玩家的LevelProgressMap）| @return 加载成功返回true 
	UFUNCTION(BlueprintPure, Category = "GameInstance|存档管理|核心操作", meta=(DisplayName="从本地加载玩家数据"))
	bool LoadPlayerDataFromLocal();

	// 自动保存（先同步当前玩家进度，再保存）
	UFUNCTION(BlueprintCallable, Category = "GameInstance|存档管理|自动保存", meta=(DisplayName="退出自动保存"))
	void AutoSaveCurrentPlayer();

	//切换玩家,从本地加载选定玩家数据到gameinstance,保存原有的玩家数据到本地
	UFUNCTION(BlueprintCallable, Category = "GameInstance|当前玩家|核心操作", meta=(DisplayName="设置当前选中玩家"))
	bool SetCurrentSelectedPlayer(int32 PlayerID);
	
	//新增玩家存档（初始化默认关卡进度）
	UFUNCTION(BlueprintCallable, Category = "GameInstance|当前玩家|核心操作", meta=(DisplayName="新增玩家存档"))
	int32 AddNewPlayer(const FString& PlayerName = TEXT(""));
	
	// 全局通用 2D 音效播放函数（BlueprintCallable 支持全项目蓝图/代码调用）
	UFUNCTION(BlueprintCallable, Category = "Global|Sound")
	void LoadAndPlaySound2D(
		const FString& SoundPath = TEXT("/Game/MyBoxGame/Sounds/SoundEffects/UI/Botton_Sound.Botton_Sound"), 
		float Volume = 0.8f
	);//默认值是bottom点击的音效
	
protected:
	/** 游戏启动初始化：加载存档→初始化当前玩家 */
	virtual void Init() override;
	
	
private:
	UPROPERTY()
	UMySaveGame* CurrentSaveGameInstance = nullptr;
	
	//生成唯一玩家ID（遍历PlayerSaveDatas，取最大ID+1）
	int32 GenerateUniquePlayerID() const;

	//根据PlayerID查找玩家在PlayerSaveDatas中的索引 
	int32 FindPlayerIndexByID(int32 PlayerID) const;

	//同步当前LevelProgressMap到存档数组中对应玩家的PlayerLevelDatas 
	bool SyncLevelProgressToPlayerSaveData();

	//根据PlayerID，将存档数组中玩家的PlayerLevelDatas同步到全局LevelProgressMap
	bool SyncPlayerSaveDataToLevelProgress(int32 PlayerID);
};

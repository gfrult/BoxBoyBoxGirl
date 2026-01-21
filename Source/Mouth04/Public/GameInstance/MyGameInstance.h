// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

class AABoxBot;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnP1RemainingBoxNumberChanged, int32, value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnP2RemainingBoxNumberChanged, int32, value);

UENUM(BlueprintType) 
enum class EGlobalPlayerType : uint8
{
	None,       
	Sheep,     
	Pig,     
	Cow    
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
	UPROPERTY()
	EG_Widget G_WidgetChose = EG_Widget::Start;//标记需要加载的界面,初始化为start界面
	
	

	//P1

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "P1AnimalConfig")
	TSubclassOf<AABoxBot> G_P1SelectedClass; //存储AABoxBot类的子类型
	
	UPROPERTY(BlueprintReadWrite, Category = "GlobalData")
	EGlobalPlayerType G_P1PlayerType = EGlobalPlayerType::Sheep;//在选择皮肤阶段进行 赋值,需要替换

	
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
};

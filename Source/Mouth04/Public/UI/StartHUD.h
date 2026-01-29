// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StartHUD.generated.h"

/**
 * 
 */


enum class EG_Widget : uint8;
class UMyGameInstance;
class UStartUserWidget;
class USelsectPlayerWidget;
class UChoseSkinWidget;
class UChoseMapWidget;
//class UExitGameWidget;



UCLASS()
class MOUTH04_API AStartHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UStartUserWidget> StartWidget;
	UPROPERTY()
	TObjectPtr<USelsectPlayerWidget> ChosePlayerWidget;	
	UPROPERTY()
	TObjectPtr<UChoseSkinWidget> ChoseSkinWidget;	
	UPROPERTY()
	TObjectPtr<UChoseMapWidget> ChoseMapWidget;
	UPROPERTY()
	FString WidgetPath;
	// BGM 相关成员变量
	UPROPERTY(EditDefaultsOnly, Category = "BGM|Config") // EditDefaultsOnly：仅在编辑器默认值中修改
	FString BgmPath = TEXT("/Game/MyBoxGame/Sounds/Music/UIChose_BGM.UIChose_BGM");
	UPROPERTY()
	FTimerHandle BgmLoopTimerHandle; // 命名句柄，有实际内存地址，支持引用传递

public:
	virtual void BeginPlay() override;

	void PlayLoopBGM();
	
	void OnBgmPlayCallback();	
	
	// 核心函数：根据 GameInstance 枚举加载对应 Widget
	void LoadWidgetByGameInstanceEnum();
	
	// 创建并显示主菜单（默认 Start 界面）
	void CreateStartMenu();

	


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StartHUD.generated.h"

/**
 * 
 */


enum class EG_Widget : uint8;
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
	
	
	
	/*
	UPROPERTY()
	TObjectPtr<UExitGameWidget> ExitGameWidget;
	*/
	UPROPERTY()
	FString WidgetPath;


public:
	virtual void BeginPlay() override;
	
	// 核心函数：根据 GameInstance 枚举加载对应 Widget
	void LoadWidgetByGameInstanceEnum();
	
	// 辅助函数：隐藏所有已显示的 Widget
	//void HideAllWidgets();
	
	// 创建并显示主菜单（默认 Start 界面）
	void CreateStartMenu();
	
};

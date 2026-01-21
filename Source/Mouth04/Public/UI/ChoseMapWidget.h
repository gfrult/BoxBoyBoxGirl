// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChoseMapWidget.generated.h"

/**
 * 
 */

class USelsectPlayerWidget;
class AMainMenuGameMode;
class UButton;
class UMyGameInstance;


UCLASS()
class MOUTH04_API UChoseMapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_InMap;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_ChosePlayer;
	
	UPROPERTY()
	USelsectPlayerWidget* ChosePlayersWidget = nullptr;
	
	UMyGameInstance* GameInstance;
	
	UFUNCTION()
	void OnClickedInMap();
	
	UFUNCTION()
	void OnClickedChosePlayer();
	
	// 工具函数：安全获取自定义GameMode实例
	AMainMenuGameMode* GetCustomGameMode();
	
};

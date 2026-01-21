// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartUserWidget.generated.h"

/**
 * 
 */

class USelsectPlayerWidget;
class UButton;
class UMyGameInstance;

UCLASS()
class MOUTH04_API UStartUserWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void StartGame();
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Start;
	
	UPROPERTY()
	USelsectPlayerWidget* ChosePlayersWidget = nullptr;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_clipping;
	
};

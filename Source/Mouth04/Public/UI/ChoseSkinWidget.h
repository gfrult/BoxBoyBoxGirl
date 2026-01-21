// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChoseSkinWidget.generated.h"

/**
 * 
 */

class UChoseMapWidget;
class USelsectPlayerWidget;
class UButton;
class UMyGameInstance;
class USelsectPlayerWidget;


UCLASS()
class MOUTH04_API UChoseSkinWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Next;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Up;
	
	UMyGameInstance* GameInstance;
	
	UFUNCTION()
	void OnClickedNext();
	
	UFUNCTION()
	void OnClickedUp();
	
		
	UPROPERTY()
	UChoseMapWidget* ChoseMapWidget = nullptr;
	
	UPROPERTY()
	USelsectPlayerWidget* ChosePlayersWidget = nullptr;
	
	
};

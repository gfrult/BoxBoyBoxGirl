// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExitWidget.generated.h"
class UMyGameInstance;
class UWidgetAnimation;
class UButton;
/**
 * 
 */
UCLASS()
class MOUTH04_API UExitWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_MainUI;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_GoOn;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Exit;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_Shine;	
	
	UMyGameInstance* GI;
	
public:
	UFUNCTION()
	void OnClickedMainUI();
	UFUNCTION()
	void OnClickedGoOn();
	UFUNCTION()
	void OnClickedExit();
	
	UFUNCTION()
	void PauseGame();
	UFUNCTION()
	void ResumeGame();
	UFUNCTION()
	void ReplayShineAnimation();
	
};

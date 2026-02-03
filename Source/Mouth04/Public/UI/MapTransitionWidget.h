// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapTransitionWidget.generated.h"

/**
 * 
 */
UCLASS()
class MOUTH04_API UMapTransitionWidget : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_Tran;
	
	FTimerHandle SelfDestroyTimerHandle;


	UFUNCTION()
	void AutoDestroySelf();
	
	UFUNCTION()
	void SetTimer1();
	
	UPROPERTY(EditAnywhere, Category = "Transition Self Life")
	float SelfLifeDuration = 3.0f;
};



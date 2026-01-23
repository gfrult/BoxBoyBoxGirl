// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LockWidget.generated.h"


class UImage;
class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class MOUTH04_API ULockWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_LockShake;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnLock;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_UnLock;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_UnlockMiss;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Shake;
	
	UFUNCTION()
	void Unlock();
	
	UFUNCTION()
	void LockShake();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelsectPlayerWidget.generated.h"


/**
 * 
 */
class UWidgetAnimation;
class UButton;
struct FTimerHandle;
class UImage;
class UChoseSkinWidget;
class UMyGameInstance;

UCLASS()
class MOUTH04_API USelsectPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_Chose_One;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_Chose_Two;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_One01;	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_TWO01;	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_Next;	
	
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_One;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Two;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Next;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_OneBGLight;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TwoBGLight;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_D01;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_D02;
	
	UPROPERTY()
	UChoseSkinWidget* ChoseSkinWidget = nullptr;
	
	UFUNCTION()
	void ChoseOne();	
	
	UFUNCTION()
	void ChoseTwo();	
	
	UFUNCTION()
	void OnHoveredOne();
	
	UFUNCTION()
	void OnHoveredTwo();
	
	UFUNCTION()
	void OnUnhoveredOne();
	
	UFUNCTION()
	void OnUnhoveredTwo();
	
	UFUNCTION()
	void OnClickedNext();
	
	UMyGameInstance* GameInstance;
	
private:

	// 悬停定时器（控制每2秒播放一次Anim_One01）
	FTimerHandle TimerHandle_One;
	// 标记是否处于Button_One悬停状态
	bool bIsHoveringOne = false;

	FTimerHandle TimerHandle_Two;
	bool bIsHoveringTwo = false;

	bool bChosed = false;
	
	// 封装播放动画的函数（复用逻辑）
	void PlayAnimationLoop(UWidgetAnimation* TargetAnim, FTimerHandle& TimerHandle, float LoopInterval);
	// 停止动画循环（只停定时器，不中断当前动画）
	void StopAnimationLoop(FTimerHandle& TimerHandle);

	
};

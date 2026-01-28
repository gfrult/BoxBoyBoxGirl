// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameInstance/MyGameInstance.h"
#include "ChoseSkinWidget.generated.h"

/**
 * 
 */


class UOverlay;
class UWidgetSwitcher;
class UTextBlock;
class UImage;
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
	
	
	UPROPERTY()
	int32 CurrentPlayer = 1;
	UPROPERTY()
	EGlobalPlayerType CurrentP1Type = EGlobalPlayerType::Sheep;
	UPROPERTY()
	EGlobalPlayerType CurrentP2Type = EGlobalPlayerType::Pig;
	UPROPERTY()
	bool bIsTowPlayer;
	UPROPERTY()
	bool bCanNext;

	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Next;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Up;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Change;
	
	UPROPERTY(meta=(BindWidget))
    UWidgetSwitcher* WidgetSwitcher_PlayerMode;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI|Overlay")
	UOverlay* Overlay_Skin;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Pig;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Sheep;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Cow;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Chick;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Cat;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Anim;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_P1Chose;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_P2Chose;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_ToP1;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_ToP2;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_Next;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_CannotNext;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI|Text")
	UTextBlock* TextBlock_P1;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI|Text")
	UTextBlock* TextBlock_P2;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI|Text")
	UTextBlock* TextBlock_ChoseP1;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI|Text")
	UTextBlock* TextBlock_ChoseP2;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_ChoseP1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_ChoseP2;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_LockAnimal;
	
	UMyGameInstance* GI;
	
	UPROPERTY()
	UChoseMapWidget* ChoseMapWidget = nullptr;
	
	UPROPERTY()
	USelsectPlayerWidget* ChosePlayersWidget = nullptr;
	
	UFUNCTION()
	void OnClickedNext();
	
	UFUNCTION()
	void OnClickedUp();
	
	UFUNCTION()
	void OnClickedChange();
	
	UFUNCTION()
	void OnClickedPig();
	UFUNCTION()
	void OnClickedSheep();
	UFUNCTION()
	void OnClickedCow();
	UFUNCTION()
	void OnClickedChick();
	UFUNCTION()
	void OnClickedCat();
	UFUNCTION()
	void OnClickedAnim();
	
	//工具函数修改GameInstance中记录P1/P2类型
	UFUNCTION()
	void ChangePlayerTypeInGI(int32 CurrentPlayerNumber,EGlobalPlayerType PlayerType);
	
	// 工具：控件独立的 Padding 设置函数
	UFUNCTION(BlueprintCallable, Category="UI|Skin|Padding")
	void SetImageChoseP1Padding(float Left, float Top, float Right, float Bottom);
	UFUNCTION(BlueprintCallable, Category="UI|Skin|Padding")
	void SetImageChoseP2Padding(float Left, float Top, float Right, float Bottom);
	UFUNCTION(BlueprintCallable, Category="UI|Skin|Padding")
	void SetTextBlockP1Padding(float Left, float Top, float Right, float Bottom);
	UFUNCTION(BlueprintCallable, Category="UI|Skin|Padding")
	void SetTextBlockP2Padding(float Left, float Top, float Right, float Bottom);
		

	

	
	
};

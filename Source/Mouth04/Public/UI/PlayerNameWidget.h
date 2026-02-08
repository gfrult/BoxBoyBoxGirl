// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerNameWidget.generated.h"


class UStartUserWidget;
class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class MOUTH04_API UPlayerNameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_NameX; // 显示玩家昵称的文本框

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Name; // 选择玩家的按钮
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_Chose;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_BG;
	
	// 设置选中图片显隐的接口
	UFUNCTION(BlueprintCallable, Category = "PlayerNameWidget")
	void SetChoseImageVisibility(bool bShowImage);
	
	
	UFUNCTION()
	void OncClickedNameX();
	
	UFUNCTION(BlueprintCallable, Category = "PlayerNameWidget")
	void SetPlayerData(const FString& PlayerName, int32 InPlayerID);
	
	UFUNCTION()
	void OnHoveredNameX();	
	
	UFUNCTION()
	void OnUnhoveredNameX();

	UPROPERTY()
	int32 PlayerID_X = 0;	
	
};

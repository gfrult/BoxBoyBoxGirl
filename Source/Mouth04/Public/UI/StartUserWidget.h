// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartUserWidget.generated.h"

/**
 * 
 */

class UPlayerNameWidget;
class UTextBlock;
class UEditableTextBox;
class UWrapBox;
class UCanvasPanel;
class USelsectPlayerWidget;
class UButton;
class UMyGameInstance;

UCLASS()
class MOUTH04_API UStartUserWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Start;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Sure;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_NewPlayer;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_ChangePlayName;
	
	UPROPERTY()
	USelsectPlayerWidget* ChosePlayersWidget = nullptr;
	
	UPROPERTY()
	UCanvasPanel* CanvasPanel_Name;

	UPROPERTY()
	UWrapBox* WrapBox_Name;

	UPROPERTY()
	UEditableTextBox* EditableTextBox_Name;
	
	UPROPERTY()
	UTextBlock* TextBlock_ChangeName;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_clipping;
	
	UPROPERTY()
	FString PlayerNickName;
	
	UFUNCTION()
    void StartGame();
	
	UFUNCTION()
	void OncClickChangePlayName();
	
	UFUNCTION()
	void OncClickNewName();
	
	UFUNCTION()
	void OncClickSure();
	
	UFUNCTION()
	void UpdatePlayerNameText();
	
	//加载并显示所有玩家数据（动态生成U_PlayerName子项） 
	UFUNCTION(BlueprintCallable, Category = "StartUserWidget|PlayerList")
	void LoadAndDisplayAllPlayers();
	
	UFUNCTION(BlueprintCallable, Category = "PlayerList")
	void RefreshSelectedImage(); // 刷新选中图片（核心无委托函数）
	
	
	//工具函数
	UFUNCTION()
	void PauseGame();
	UFUNCTION()
	void ResumeGame();
	// 校验输入的昵称是否合法（非空 + ≤6个汉字）| @return 合法返回true
	bool ValidatePlayerName(const FString& InputName);

	
	
	
private:
	/** 存储动态生成的PlayerNameWidget（防GC回收） */
	UPROPERTY()
	TArray<TObjectPtr<UPlayerNameWidget>> SpawnedPlayerWidgets;

	/** U_PlayerName蓝图类引用（指向U_PlayerName蓝图） */
	UPROPERTY(EditDefaultsOnly, Category = "Widget Settings")
	TSubclassOf<UPlayerNameWidget> PlayerNameWidgetClass;
	
	
};

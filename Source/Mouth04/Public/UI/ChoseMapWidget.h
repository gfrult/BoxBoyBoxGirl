// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChoseMapWidget.generated.h"

/**
 * 
 */

class UStarWidget;
class UWidgetSwitcher;
class UImage;
class ULockWidget;
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
	
	UPROPERTY(meta=(BindWidget))
	UWidgetSwitcher* WidgetSwitcher_PlayerN;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Solo1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Solo11;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Solo2;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Solo21;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Solo3;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> Button_Map1;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Map2;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Map21;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Map3;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Map4;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Map41;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Map5;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Map6;
	
	UPROPERTY(meta = (BindWidget))
	ULockWidget* U_MapLock1;
	UPROPERTY(meta = (BindWidget))
	ULockWidget* U_MapLock2;
	UPROPERTY(meta = (BindWidget))
	ULockWidget* U_MapLock3;
	UPROPERTY(meta = (BindWidget))
	ULockWidget* U_MapLock4;
	UPROPERTY(meta = (BindWidget))
	ULockWidget* U_MapLock5;
	UPROPERTY(meta = (BindWidget))
	ULockWidget* U_MapLock6;
	UPROPERTY(meta = (BindWidget))
	ULockWidget* U_MapLock_O1;
	UPROPERTY(meta = (BindWidget))
	ULockWidget* U_MapLock_O2;
	UPROPERTY(meta = (BindWidget))
	ULockWidget* U_MapLock_O3;
	
	UPROPERTY(meta = (BindWidget))
	UStarWidget* U_Star_1;
	UPROPERTY(meta = (BindWidget))
	UStarWidget* U_Star_2;
	UPROPERTY(meta = (BindWidget))
	UStarWidget* U_Star_3;
	UPROPERTY(meta = (BindWidget))
	UStarWidget* U_Star_4;
	UPROPERTY(meta = (BindWidget))
	UStarWidget* U_Star_5;
	UPROPERTY(meta = (BindWidget))
	UStarWidget* U_Star_6;
	UPROPERTY(meta = (BindWidget))
	UStarWidget* U_Star_O1;
	UPROPERTY(meta = (BindWidget))
	UStarWidget* U_Star_O2;
	UPROPERTY(meta = (BindWidget))
	UStarWidget* U_Star_O3;
	
	
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnlockMap1;	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnlockMap2;	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnlockMap3;	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnlockMap4;	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnlockMap5;	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnlockMap6;	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnlockSolo1;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnlockSolo2;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_UnlockSolo3;
	
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_ChoseShare;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_Next;
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_ChangePlayerMod;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TwoM1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TwoM2;	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TwoM3;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TwoM4;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TwoM5;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TwoM6;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_OneM1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_OneM2;	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_OneM3;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TowGray1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TowGray2;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TowGray3;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TowGray4;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TowGray5;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_TowGray6;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_OneGray1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_OneGray2;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_OneGray3;
	
	
	
	UPROPERTY()
	USelsectPlayerWidget* ChosePlayersWidget = nullptr;
	
	bool bNextInMap=false;
	FName MapName;	
	FString ModeStr;
	
	UFUNCTION()
	void OnClickedInMap();
	
	UFUNCTION()
	void OnClickedChosePlayer();
	UFUNCTION()
    void OnHoveredChosePlayer();

	
	UFUNCTION()
	void OnClickedMapBotton(int32 Index);
	
	UFUNCTION()
	void OnClickedTowMap1();
	UFUNCTION()
	void OnClickedTowMap2();
	UFUNCTION()
	void OnClickedTowMap3();
	UFUNCTION()
	void OnClickedTowMap4();
	UFUNCTION()
	void OnClickedTowMap5();
	UFUNCTION()
	void OnClickedTowMap6();
	UFUNCTION()
	void OnClickedOneSolo1();
	UFUNCTION()
	void OnClickedOneSolo2();
	UFUNCTION()
	void OnClickedOneSolo3();
	
	
	UFUNCTION()
	void InitializeMapLock(int32 MapIndex);
	
	UFUNCTION()
	void ShakeLockedMap(int32 MapIndex);
	UFUNCTION()
	void ChoseMapAnim(int32 MapIndex);
	

	// 工具函数：安全获取自定义GameMode实例
	AMainMenuGameMode* GetCustomGameMode();
	// 工具函数：MapName字符串拼接
	void CombineMapName(int32 MapIndex);
	
};

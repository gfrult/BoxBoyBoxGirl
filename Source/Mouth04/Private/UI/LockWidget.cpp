// Fill out your copyright notice in the Description page of Project Settings.


#include "LockWidget.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"

void ULockWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	
	Image_UnLock->SetVisibility(ESlateVisibility::Hidden);
	
	Image_Shake->SetVisibility(ESlateVisibility::Visible);
	
}

void ULockWidget::Unlock()
{
	Image_Shake->SetVisibility(ESlateVisibility::Hidden);
	Image_UnlockMiss->SetVisibility(ESlateVisibility::Visible);
	Image_UnLock->SetVisibility(ESlateVisibility::Visible);
	if (Anim_UnLock)
	{
		PlayAnimation(Anim_UnLock);
		UE_LOG(LogTemp, Log, TEXT("锁UI:播放解锁动画"));
	}
}

void ULockWidget::LockShake()
{
	Image_UnLock->SetVisibility(ESlateVisibility::Hidden);
	Image_UnlockMiss->SetVisibility(ESlateVisibility::Hidden);
	Image_Shake->SetVisibility(ESlateVisibility::Visible);
	
	if (Anim_LockShake)
	{
		PlayAnimation(Anim_LockShake);

	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MapTransitionWidget.h"

void UMapTransitionWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(Anim_Tran);
	SetTimer1();
}

void UMapTransitionWidget::AutoDestroySelf()
{
	// 步骤1：先从视口移除自身（停止渲染，避免残留）
	if (this->IsInViewport())
	{
		this->RemoveFromViewport();
	}
	// 步骤2：标记为垃圾，交给UE GC回收内存（不可用delete！）
	this->MarkAsGarbage();
}

void UMapTransitionWidget::SetTimer1()
{
	UWorld* CurrentWorld = this->GetWorld();
	if (IsValid(CurrentWorld))
	{
		// 先清除旧定时器（避免重复显示导致多个定时器生效）
		CurrentWorld->GetTimerManager().ClearTimer(SelfDestroyTimerHandle);

		// 启动定时器，到期后调用AutoDestroySelf
		CurrentWorld->GetTimerManager().SetTimer(
			SelfDestroyTimerHandle,
			FTimerDelegate::CreateUObject(this, &UMapTransitionWidget::AutoDestroySelf),
			SelfLifeDuration, // 默认2.5秒，可在蓝图中修改
			false // 仅执行一次
		);
	}
}

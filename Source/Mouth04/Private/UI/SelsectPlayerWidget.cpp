// Fill out your copyright notice in the Description page of Project Settings.


#include "SelsectPlayerWidget.h"
#include "Blueprint/UserWidget.h"
#include "ChoseSkinWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Widget.h"
#include "GameFramework/PlayerController.h"


void USelsectPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 绑定按钮点击事件（核心交互逻辑）
	if (Button_One)
	{
		// 点击Button_One这个按钮,会执行这个函数
		Button_One->OnClicked.AddDynamic(this, &USelsectPlayerWidget::ChoseOne);
		//悬停时
		Button_One->OnHovered.AddDynamic(this, &USelsectPlayerWidget::OnHoveredOne);
		Button_One->OnUnhovered.AddDynamic(this, &USelsectPlayerWidget::OnUnhoveredOne);
	}
	if (Button_Two)
	{
		// 点击Button_Two这个按钮,会执行这个函数
		Button_Two->OnClicked.AddDynamic(this, &USelsectPlayerWidget::ChoseTwo);
		//悬停时
		Button_Two->OnHovered.AddDynamic(this, &USelsectPlayerWidget::OnHoveredTwo);
		Button_Two->OnUnhovered.AddDynamic(this, &USelsectPlayerWidget::OnUnhoveredTwo);
	}
	
	if (Button_Next)
	{
		Button_Next->OnClicked.AddDynamic(this, &USelsectPlayerWidget::OnClickedNext);
	}
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	bChosed = false;
	if (Image_OneBGLight)
	{
		Image_OneBGLight->SetVisibility(ESlateVisibility::Hidden);
	}
	if (Image_TwoBGLight)
	{
		Image_TwoBGLight->SetVisibility(ESlateVisibility::Hidden);
	}
}
	
void USelsectPlayerWidget::NativeDestruct()
{
	// 销毁Widget时清理定时器，防止内存泄漏
	StopAnimationLoop(TimerHandle_One);
	StopAnimationLoop(TimerHandle_Two);
	UE_LOG(LogTemp, Log, TEXT("UMG:玩家数量选择widget实例 被销毁"));
	Super::NativeDestruct();
}	

void USelsectPlayerWidget::ChoseOne()
{
	Image_D02->SetVisibility(ESlateVisibility::Hidden);
	Image_D01->SetVisibility(ESlateVisibility::Visible);
	if (bChosed ==false)
	{
		PlayAnimation(Anim_Next);//播放动画
	}
	if (Anim_Chose_One)
	{
		bChosed =true;
        PlayAnimation(Anim_Chose_One); // UE内置函数：播放Widget动画				
	}
	if (GameInstance)
	{
		GameInstance->bIsTwoPlayerMode = false;
		GameInstance->LoadAndPlaySound2D();//播放音效
	}
}

void USelsectPlayerWidget::ChoseTwo()
{
	Image_D01->SetVisibility(ESlateVisibility::Hidden);
	Image_D02->SetVisibility(ESlateVisibility::Visible);	
	if (bChosed ==false)
	{
		PlayAnimation(Anim_Next);//播放动画
	}
	if (Anim_Chose_Two)
	{
		bChosed =true;
		PlayAnimation(Anim_Chose_Two); // UE内置函数：播放Widget动画
	}
	if (GameInstance)
	{
		GameInstance->bIsTwoPlayerMode = true;
		GameInstance->LoadAndPlaySound2D();//播放音效
	}
}

void USelsectPlayerWidget::OnHoveredOne()
{
	if (Image_OneBGLight)
	{
		Image_OneBGLight->SetVisibility(ESlateVisibility::Visible);
	}
	bIsHoveringOne = true;
	// 动画时长2秒，因此循环间隔设为2秒
	const float LoopInterval = 2.0f;
	// 立即播放一次动画，然后启动定时器循环播放
	PlayAnimationLoop(Anim_One01, TimerHandle_One, LoopInterval);
}

void USelsectPlayerWidget::OnHoveredTwo()
{
	if (Image_TwoBGLight)
	{
		Image_TwoBGLight->SetVisibility(ESlateVisibility::Visible);
	}
	bIsHoveringTwo = true;
	const float LoopInterval = 2.0f;
	PlayAnimationLoop(Anim_TWO01, TimerHandle_Two, LoopInterval);
}

void USelsectPlayerWidget::OnUnhoveredOne()
{
	Image_OneBGLight->SetVisibility(ESlateVisibility::Hidden);
	bIsHoveringOne = false;
	// 停止定时器（不再触发新的动画），当前动画继续播放完毕
	StopAnimationLoop(TimerHandle_One);
}

void USelsectPlayerWidget::OnUnhoveredTwo()
{
	Image_TwoBGLight->SetVisibility(ESlateVisibility::Hidden);
	bIsHoveringTwo = false;
	StopAnimationLoop(TimerHandle_Two);
}

void USelsectPlayerWidget::OnClickedNext()
{
	if (bChosed==false) return;
	UE_LOG(LogTemp, Log, TEXT("UMG:selectplayerwidget 点击next按键"));
	if (!ChoseSkinWidget)
	{
		TSubclassOf<UChoseSkinWidget> WidgetClass = LoadClass<UChoseSkinWidget>
		(
		nullptr, 
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_ChoseSkin.U_ChoseSkin_C'")
		);
		if (WidgetClass)
		{
			// 先通过GetWorld()获取PlayerController
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				UE_LOG(LogTemp, Log, TEXT("UMG: 创建皮肤选择界面"));
				ChoseSkinWidget = CreateWidget<UChoseSkinWidget>(PC, WidgetClass);//通过玩家控制器来创建widget
			}
			if (ChoseSkinWidget)
			{
				ChoseSkinWidget->AddToViewport(); // 显示到屏幕
			}
		}
	}
	else
	{
		ChoseSkinWidget->AddToViewport(); // 显示到屏幕
	}	
	GameInstance->G_WidgetChose = EG_Widget::ChoseSkin;//标记进入皮肤选择界面
	GameInstance->LoadAndPlaySound2D();//播放音效
	this->RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("UMG: SelectPlayerWidget已从视口移除！"));
}



// --- 播放循环动画 ---
void USelsectPlayerWidget::PlayAnimationLoop(UWidgetAnimation* TargetAnim, FTimerHandle& TimerHandle, float LoopInterval)
{
	if (!TargetAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("目标动画为空，无法播放！"));
		return;
	}
	// 1. 先播放一次当前动画（如果未在播放）
	if (!IsAnimationPlaying(TargetAnim))
	{
		PlayAnimation(TargetAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	}
	// 2. 启动定时器：每LoopInterval秒触发一次动画播放
	GetWorld()->GetTimerManager().SetTimer
	(
		TimerHandle, 
		[this, TargetAnim, LoopInterval]() // 匿名lambda表达式
		{
			// 只有仍处于悬停状态时，才播放下一次动画
			if ((TargetAnim == Anim_One01 && bIsHoveringOne) || (TargetAnim == Anim_TWO01 && bIsHoveringTwo))
			{
				// 停止当前可能未播放完的动画（可选，根据需求），重新播放
				StopAnimation(TargetAnim);
				PlayAnimation(TargetAnim, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
			}
		},
		LoopInterval, // 循环间隔（2秒）
		true // 重复触发
	);
}

// --- 停止动画循环（仅停定时器） ---
void USelsectPlayerWidget::StopAnimationLoop(FTimerHandle& TimerHandle)
{
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(TimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}


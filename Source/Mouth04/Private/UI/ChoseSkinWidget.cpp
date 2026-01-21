// Fill out your copyright notice in the Description page of Project Settings.


#include "ChoseSkinWidget.h"
#include "ChoseMapWidget.h"
#include "SelsectPlayerWidget.h"
#include "Components/Button.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


void UChoseSkinWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Button_Next)
	{
		Button_Next->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedNext);
	}
	if (Button_Up)
	{
		Button_Up->OnClicked.AddDynamic(this, &UChoseSkinWidget::OnClickedUp);
	}
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void UChoseSkinWidget::OnClickedNext()
{

	UE_LOG(LogTemp, Log, TEXT("UMG:点击下一页:加载地图选择界面"));
	if (!ChoseMapWidget)
	{
		TSubclassOf<UChoseMapWidget> WidgetClass = LoadClass<UChoseMapWidget>
		(
		nullptr, 
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_ChoseMap.U_ChoseMap_C'")
		);
		if (WidgetClass)
		{
			// 先通过GetWorld()获取PlayerController
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				ChoseMapWidget = CreateWidget<UChoseMapWidget>(PC, WidgetClass);
			}
			if (ChoseMapWidget)
			{
				ChoseMapWidget->AddToViewport(); // 显示到屏幕
			}
		}
	}
	else
	{
		ChoseMapWidget->AddToViewport(); // 显示到屏幕
	}
	GameInstance->G_WidgetChose = EG_Widget::ChoseMap;//标记进入游戏选择界面
	this->RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("UMG: chose skin 已从视口移除！"));
}

void UChoseSkinWidget::OnClickedUp()
{
	UE_LOG(LogTemp, Log, TEXT("UMG:点击返回上一级:玩家数量选择界面"));
	if (!ChosePlayersWidget)
	{
		TSubclassOf<USelsectPlayerWidget> WidgetClass = LoadClass<USelsectPlayerWidget>
		(
		nullptr, 
		TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_SelectPlayer.U_SelectPlayer_C'")
		);
		if (WidgetClass)
		{
			// 先通过GetWorld()获取PlayerController
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				ChosePlayersWidget = CreateWidget<USelsectPlayerWidget>(PC, WidgetClass);
			}
			if (ChosePlayersWidget)
			{
				ChosePlayersWidget->AddToViewport(); // 显示到屏幕
			}
		}
	}
	else
	{
		ChosePlayersWidget->AddToViewport(); // 显示到屏幕
	}	
	GameInstance->G_WidgetChose = EG_Widget::ChosePlayer;//标记进入游戏选择界面
	this->RemoveFromParent();
	UE_LOG(LogTemp, Log, TEXT("UMG: chose skin 已从视口移除！"));
}

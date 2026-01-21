// Fill out your copyright notice in the Description page of Project Settings.


#include "StartUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "SelsectPlayerWidget.h"
#include "Components/Button.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Widget.h"
#include "GameFramework/PlayerController.h"

void UStartUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
		// 绑定按钮点击事件（核心交互逻辑）
    	if (Button_Start)
    	{
    		// 点击Button_Start这个按钮,会执行这个函数
    		Button_Start->OnClicked.AddDynamic(this, &UStartUserWidget::StartGame);
    	}
	
		// 播放主菜单初始化动画（绑定的MainMenuAnim）
		if (Anim_clipping)
		{
			PlayAnimation(Anim_clipping); // UE内置函数：播放Widget动画
		}
	
}

void UStartUserWidget::StartGame()
{
	//获取 UE 全局唯一的 GameInstance 实例，并将其转换为我们自定义的UMyGameInstance类型，从而访问其中的全局变量（如枚举、数值）
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	UE_LOG(LogTemp, Log, TEXT("UMG:点击开始游戏"));
	
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
	UE_LOG(LogTemp, Log, TEXT("StartGame: StartUserWidget已从视口移除！"));
	
}

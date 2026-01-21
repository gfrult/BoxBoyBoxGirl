// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyHUD.h"
#include "UI/MyUserWidget.h"

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
	ShowMainMenu();
	GetOwningPlayerController()->SetShowMouseCursor(true);//显示鼠标
	
}


void AMyHUD::ShowMainMenu()
{
	
	
	if (!MainMenuWidget)// 如果主菜单Widget还没创建，就创建并显示
	{
		// 加载Widget蓝图类
		TSubclassOf<UMyUserWidget> WidgetClass = LoadClass<UMyUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UMG/U_Main.U_Main_C'"));
		if (WidgetClass)//如果这个路径下的蓝图能成功加载出来
		{
			// 创建Widget实例
			MainMenuWidget = CreateWidget<UMyUserWidget>(GetOwningPlayerController(), WidgetClass);
			if (MainMenuWidget)
			{
				MainMenuWidget->AddToViewport();  // 显示到屏幕
			}
		}
	}
}

void AMyHUD::HideMainMenu()
{

	if (MainMenuWidget)	// 如果主菜单Widget存在，就隐藏/移除
	{
		//MainMenuWidget->RemoveFromViewport();  // 从屏幕移除
		// 销毁Widget（根据需求，也可保留实例后续复用）
		// MainMenuWidget->Destroy();
		// MainMenuWidget = nullptr;
	}
}

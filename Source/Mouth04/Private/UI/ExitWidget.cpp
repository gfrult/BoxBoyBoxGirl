// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitWidget.h"
#include "Components/Button.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"


void UExitWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_MainUI->OnClicked.AddDynamic(this, &UExitWidget::OnClickedMainUI);
	Button_Exit->OnClicked.AddDynamic(this, &UExitWidget::OnClickedExit);
	Button_GoOn->OnClicked.AddDynamic(this, &UExitWidget::OnClickedGoOn);
	PlayAnimation(Anim_Shine);
	GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->SavePlayerDataToLocal();//更新存档
}

void UExitWidget::OnClickedMainUI()
{
	GI->bIsShowSetting=false;
	GI->LoadAndPlaySound2D();
	GI->G_WidgetChose = EG_Widget::Start;//标记进入start页面
	UGameplayStatics::OpenLevel(this, "M_Menu");
	this->RemoveFromParent();
	ResumeGame();
}

void UExitWidget::OnClickedGoOn()
{
	GI->bIsShowSetting=false;
	ResumeGame();
	GI->LoadAndPlaySound2D();
	this->RemoveFromParent();
}

void UExitWidget::OnClickedExit()
{
	GI->LoadAndPlaySound2D();
	GI->SavePlayerDataToLocal();//更新存档
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	// 步骤2：调用 UE 标准退出函数 QuitGame()
	UKismetSystemLibrary::QuitGame(
		GetWorld(),        // 世界上下文
		PlayerController,  // 玩家控制器
		EQuitPreference::Quit, // 退出偏好：直接退出（打包后生效）
		true               // 是否确认退出（true 表示无需额外确认）
	);
}

// 封装暂停游戏逻辑
void UExitWidget::PauseGame()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("【ExitWidget】无法暂停游戏：世界上下文无效！"));
		return;
	}

	// 核心函数：设置游戏为暂停状态
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeUIOnly()); // 仅允许 UI 输入，禁止游戏操作
	}

	UE_LOG(LogTemp, Log, TEXT("【ExitWidget】游戏已暂停！"));
}

// 封装恢复游戏逻辑
void UExitWidget::ResumeGame()
{
	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("【ExitWidget】无法恢复游戏：世界上下文无效！"));
		return;
	}

	// 核心函数：设置游戏为运行状态
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PlayerController))
	{
		PlayerController->SetInputMode(FInputModeGameOnly()); // 仅允许游戏输入，禁止 UI 操作
	}

	UE_LOG(LogTemp, Log, TEXT("【ExitWidget】游戏已恢复运行！"));
}

void UExitWidget::ReplayShineAnimation()
{
	// 先停止当前动画（避免从中间继续播放），再从头播放
	if (IsValid(Anim_Shine))
	{
		StopAnimation(Anim_Shine); // 停止当前动画，重置到初始帧
		PlayAnimation(Anim_Shine); // 从头开始播放动画
	}
}
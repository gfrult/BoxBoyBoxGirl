// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUserWidget.h"
// UE内置头文件：包含UI动画、按钮事件相关函数
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HorizontalBox.h"
#include "Math/UnrealMathUtility.h"
#include "GameInstance/MyGameInstance.h"


void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();// 必须调用父类的NativeConstruct，保证基类逻辑执行
	
	// 播放主菜单初始化动画（绑定的MainMenuAnim）
	if (MainMenuAnim)
	{
		PlayAnimation(MainMenuAnim); // UE内置函数：播放Widget动画
	}

	// 绑定按钮点击事件（核心交互逻辑）
	if (ShowSettingBtn)
	{
		// 点击ShowSettingBtn这个按钮,会执行ShowSettingWidget这个函数
		ShowSettingBtn->OnClicked.AddDynamic(this, &UMyUserWidget::ShowSettingWidget);
	}
	
	//获取 UE 全局唯一的 GameInstance 实例，并将其转换为我们自定义的UMyGameInstance类型，从而访问其中的全局变量（如枚举、数值）
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!IsValid(GameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("UI中读取 GameInstance无效!!"));
		return;
	}
		
	// 订阅委托
	CachedGameInstance=GameInstance;
	CachedGameInstance->OnP1RemainingBoxNumberChanged.AddDynamic
	(
		this,
		&UMyUserWidget::OnRemainingBoxNumberChanged
	);
	CachedGameInstance->OnP2RemainingBoxNumberChanged.AddDynamic
	(
		this,
		&UMyUserWidget::OnP2RemainingBoxNumberChanged
	);
		
	//初始化玩家1的数值和图标
	FText P1MaxBoxNumberText = FText::FromString(FString::Printf(TEXT("%d"), GameInstance->G_P1MaxBoxNumber));
	SetTextBlockContent(TextBlock_P1MaxNum,P1MaxBoxNumberText);
	//  初始化显示当前剩余箱子数（避免UI初始为空）
	UpdateP1RemainingBoxNumberText(CachedGameInstance->GetP1RemainingBoxNumber());
	FText P1RemainingBoxNumberText = FText::FromString(FString::Printf(TEXT("%d"), GameInstance->G_P1RemainingBoxNumber));	
	SetTextBlockContent(TextBlock_P1CanUseNum,P1RemainingBoxNumberText);
	EGlobalPlayerType P1Type = GameInstance->G_P1PlayerType;
	switch (P1Type)
	{
		case EGlobalPlayerType::Sheep:
			SetImageByPath(Image_P1,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_Sheep01.UI_Sheep01'"));
			SetImageByPath(Image_P1Box,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_BoxSheepA.UI_BoxSheepA'"));
			break;
		case EGlobalPlayerType::Pig:
			SetImageByPath(Image_P1,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_Pig01.UI_Pig01'"));
			SetImageByPath(Image_P1Box,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_BoxpigA.UI_BoxpigA'"));	
			break;
		case EGlobalPlayerType::None:
			UE_LOG(LogTemp, Error, TEXT("玩家1 type为None!!!!"));
			break;
	}

	
	//初始化玩家2的数值和图标
	EGlobalPlayerType P2Type = GameInstance->G_P2PlayerType;
	if (GameInstance->bIsTwoPlayerMode==false)
	{
		HorizontalBox_P2->SetVisibility(ESlateVisibility::Hidden);//隐藏P2条形框
	}
	else
	{
		HorizontalBox_P2->SetVisibility(ESlateVisibility::Visible);
		FText P2MaxBoxNumberText = FText::FromString(FString::Printf(TEXT("%d"), GameInstance->G_P2MaxBoxNumber));
		SetTextBlockContent(TextBlock_P2MaxNum,P2MaxBoxNumberText);
		UpdateP2RemainingBoxNumberText(CachedGameInstance->GetP2RemainingBoxNumber());
		switch (P2Type)
        {
        case EGlobalPlayerType::Sheep:
        	SetImageByPath(Image_P2,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_Sheep01.UI_Sheep01'"));
        	SetImageByPath(Image_P2Box,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_BoxSheepA.UI_BoxSheepA'"));
        	break;
        case EGlobalPlayerType::Pig:
        	SetImageByPath(Image_P2,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_Pig01.UI_Pig01'"));
        	SetImageByPath(Image_P2Box,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_BoxpigA.UI_BoxpigA'"));	
        	break;
        case EGlobalPlayerType::None:
        	UE_LOG(LogTemp, Error, TEXT("玩家2 type为None!!!!"));
        	break;
		}
	}
	
}

// UI销毁：取消委托订阅（可选，AddUObject已处理，但显式取消更安全）
void UMyUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(CachedGameInstance))
	{
		CachedGameInstance->OnP1RemainingBoxNumberChanged.RemoveAll(this);
	}
}



void UMyUserWidget::ShowSettingWidget()
{
	// 这里写“显示设置界面”的具体逻辑：
	// 1. 加载/创建设置界面Widget
	// 2. 显示设置界面（AddToViewport）
	// 3. 隐藏当前主菜单（可选）
	// 示例逻辑：
	UE_LOG(LogTemp, Log, TEXT("UMG:按钮功能测试   _点击了显示设置按钮，准备显示设置界面！"));
}


// ========== 通用函数：修改任意文本块内容 ==========
void UMyUserWidget::SetTextBlockContent(UTextBlock* TargetTextBlock, const FText& NewText)
{
	// 安全判断：目标文本块为空则打印错误
	if (!IsValid(TargetTextBlock))
	{
		UE_LOG(LogTemp, Error, TEXT("UMG:目标文本块为空，无法修改文本内容！"));
		return;
	}

	// 修改文本内容
	TargetTextBlock->SetText(NewText);
	// 打印日志：包含文本块名称（方便定位是哪个控件）
	UE_LOG(LogTemp, Log, TEXT("UMG:文本块[%s]内容修改为：%s"), 
		*TargetTextBlock->GetName(), // 获取控件名称
		*NewText.ToString());
}

// ========== 通用函数：修改任意图片控件的纹理 ==========
void UMyUserWidget::SetImageByPath(UImage* TargetImage, const FString& ImagePath)
{
	// 安全判断：目标图片控件为空
	if (!IsValid(TargetImage))
	{
		UE_LOG(LogTemp, Error, TEXT("UMG:目标图片控件为空，无法修改图片！"));
		return;
	}

	// 空路径：清空图片
	if (ImagePath.IsEmpty())
	{
		TargetImage->SetBrushFromTexture(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("UMG:图片路径为空，清空图片控件[%s]"), *TargetImage->GetName());
		return;
	}

	// 加载纹理资源
	UTexture2D* TargetTexture = LoadObject<UTexture2D>(
		nullptr,
		*ImagePath,
		nullptr,
		LOAD_None
	);

	if (IsValid(TargetTexture))
	{
		TargetImage->SetBrushFromTexture(TargetTexture);
		UE_LOG(LogTemp, Log, TEXT("UMG:图片控件[%s]成功加载纹理：%s"), 
			*TargetImage->GetName(), 
			*ImagePath);
	}
	else
	{
		TargetImage->SetBrushFromTexture(nullptr);
		UE_LOG(LogTemp, Error, TEXT("UMG:图片控件[%s]加载纹理失败！路径：%s"), 
			*TargetImage->GetName(), 
			*ImagePath);
	}
}



// 委托处理函数：接收剩余箱子数并更新UI
void UMyUserWidget::OnRemainingBoxNumberChanged(int32 NewNumber)
{
	UpdateP1RemainingBoxNumberText(NewNumber);
}

void UMyUserWidget::OnP2RemainingBoxNumberChanged(int32 NewNumber)
{
	UpdateP2RemainingBoxNumberText(NewNumber);
}



// 现有箱子数更新函数
void UMyUserWidget::UpdateP1RemainingBoxNumberText(int32 NewNumber)
{
	if (IsValid(TextBlock_P1CanUseNum))
	{
		// 格式化文本（可根据UI需求调整文案）
		FText DisplayText = FText::FromString(FString::Printf(TEXT("%d"), NewNumber));
		TextBlock_P1CanUseNum->SetText(DisplayText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MyUserWidget: TextBlock_P1CanUseNum控件为空"));
	}
}

void UMyUserWidget::UpdateP2RemainingBoxNumberText(int32 NewNumber)
{
	if (IsValid(TextBlock_P2CanUseNum))
	{
		// 格式化文本（可根据UI需求调整文案）
		FText DisplayText = FText::FromString(FString::Printf(TEXT("%d"), NewNumber));
		TextBlock_P2CanUseNum->SetText(DisplayText);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MyUserWidget: TextBlock_P2CanUseNum控件为空"));
	}
}


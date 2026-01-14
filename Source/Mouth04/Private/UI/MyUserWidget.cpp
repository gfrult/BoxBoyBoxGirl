// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUserWidget.h"
// UE内置头文件：包含UI动画、按钮事件相关函数
#include "Components/Button.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"


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
	
	//初始化玩家1的数值和图标
	SetTextBlockContent(TextBlock_P1MaxNum,FText::FromString(TEXT("5")));
	SetTextBlockContent(TextBlock_P1CanUseNum,FText::FromString(TEXT("4")));
	SetImageByPath(Image_P1,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_Sheep01.UI_Sheep01'"));
	SetImageByPath(Image_P1Box,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_BoxSheepA.UI_BoxSheepA'"));
	
	if (!bP2IsValid)
	{
		SetTextBlockContent(TextBlock_P2MaxNum,FText::FromString(TEXT("5")));
		SetTextBlockContent(TextBlock_P2CanUseNum,FText::FromString(TEXT("4")));
		SetImageByPath(Image_P2,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_Pig01.UI_Pig01'"));
		SetImageByPath(Image_P2Box,TEXT("/Script/Engine.Texture2D'/Game/MyBoxGame/Textures/UI/Icons/UI_BoxpigA.UI_BoxpigA'"));
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







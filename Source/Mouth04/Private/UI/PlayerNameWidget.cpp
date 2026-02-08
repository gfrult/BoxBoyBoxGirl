// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerNameWidget.h"

#include "StartUserWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


void UPlayerNameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Name->OnClicked.AddDynamic(this, &UPlayerNameWidget::OncClickedNameX);
	Button_Name->OnHovered.AddDynamic(this, &UPlayerNameWidget::OnHoveredNameX);
	Button_Name->OnUnhovered.AddDynamic(this, &UPlayerNameWidget::OnUnhoveredNameX);
	Image_BG->SetRenderOpacity(0.5f);
	Image_Chose->SetVisibility(ESlateVisibility::Hidden);
}


void UPlayerNameWidget::SetChoseImageVisibility(bool bShowImage)
{
	if (Image_Chose)
	{
		if (bShowImage)
		{
			Image_Chose->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			Image_Chose->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}


void UPlayerNameWidget::OncClickedNameX()
{
	UMyGameInstance* GI = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	GI->LoadAndPlaySound2D();//播放音效
	GI->CurrentSelectedPlayerID = PlayerID_X;
	UE_LOG(LogTemp, Log, TEXT("点击按钮Name,当前PlayerID：%d"), PlayerID_X);
	if (GI->StartWidgetInstance)
	{
		GI->StartWidgetInstance->RefreshSelectedImage(); // 调用刷新函数
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerNameWidget::OncClickedNameX - GameInstance中未找到StartWidget实例！"));
	}
}

void UPlayerNameWidget::SetPlayerData(const FString& PlayerName, int32 InPlayerID)
{
	// 设置昵称文本
	if (TextBlock_NameX)
	{
		TextBlock_NameX->SetText(FText::FromString(PlayerName));
	}
	// 记录PlayerID
	PlayerID_X = InPlayerID;
}

void UPlayerNameWidget::OnHoveredNameX()
{
	Image_BG->SetRenderOpacity(1.0f);
}

void UPlayerNameWidget::OnUnhoveredNameX()
{
	Image_BG->SetRenderOpacity(0.5f);
}


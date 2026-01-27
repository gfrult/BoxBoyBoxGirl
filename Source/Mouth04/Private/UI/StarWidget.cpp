// Fill out your copyright notice in the Description page of Project Settings.


#include "StarWidget.h"

#include "Components/Image.h"

void UStarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	Image_S1->SetVisibility(ESlateVisibility::Hidden);
	Image_S2->SetVisibility(ESlateVisibility::Hidden);
	Image_S3->SetVisibility(ESlateVisibility::Hidden);
	Image_S4->SetVisibility(ESlateVisibility::Hidden);
	Image_S5->SetVisibility(ESlateVisibility::Hidden);
	
	PlayAnimation(Anim_Wink);
}

void UStarWidget::SetStarNumber(int32 Number)
{
	switch (Number)
	{
		case 0:
		break;
		case 1:
		Image_S1->SetVisibility(ESlateVisibility::Visible);
		break;
		case 2:
		Image_S1->SetVisibility(ESlateVisibility::Visible);
		Image_S2->SetVisibility(ESlateVisibility::Visible);
		break;
		case 3:
		Image_S1->SetVisibility(ESlateVisibility::Visible);
		Image_S2->SetVisibility(ESlateVisibility::Visible);
		Image_S3->SetVisibility(ESlateVisibility::Visible);
		break;
		case 4:
		Image_S1->SetVisibility(ESlateVisibility::Visible);
		Image_S2->SetVisibility(ESlateVisibility::Visible);
		Image_S3->SetVisibility(ESlateVisibility::Visible);
		Image_S4->SetVisibility(ESlateVisibility::Visible);	
		break;
		case 5:
		Image_S1->SetVisibility(ESlateVisibility::Visible);
		Image_S2->SetVisibility(ESlateVisibility::Visible);
		Image_S3->SetVisibility(ESlateVisibility::Visible);
		Image_S4->SetVisibility(ESlateVisibility::Visible);
		Image_S5->SetVisibility(ESlateVisibility::Visible);
		default:
		break;
	}
}

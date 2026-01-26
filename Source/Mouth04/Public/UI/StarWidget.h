// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StarWidget.generated.h"

class UImage;
class UWidgetAnimation;



UCLASS()
class MOUTH04_API UStarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidgetAnim),Transient)
	TObjectPtr<UWidgetAnimation> Anim_Wink;	
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_S1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_S2;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_S3;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_S4;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> Image_S5;
	
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void SetStarNumber(int32 Number);
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"


class UMyUserWidget;

UCLASS()
class MOUTH04_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	UPROPERTY()
	TObjectPtr<UMyUserWidget> MainMenuWidget;//前项声明,一个叫MainMenuWidget的安全指针类型


public:
	virtual void BeginPlay();
	
	void ShowMainMenu();
	
	void HideMainMenu();
};

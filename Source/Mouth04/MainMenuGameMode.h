// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class AABoxBot;
/**
 * 
 */
UCLASS()
class MOUTH04_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	AMainMenuGameMode();
	
	UFUNCTION(BlueprintCallable, Category = "Menu Logic")
	void SetTwoPlayerMode(bool bEnable);
	
	UFUNCTION(BlueprintCallable, Category = "Menu Logic")
	void ConfirmPlayer1Selection(TSubclassOf<AABoxBot> SelectedClass, EGlobalPlayerType PlayerType);
	
	UFUNCTION(BlueprintCallable, Category = "Menu Logic")
	void ConfirmPlayer2Selection(TSubclassOf<AABoxBot> SelectedClass, EGlobalPlayerType PlayerType);
	
	UFUNCTION(BlueprintCallable, Category = "Menu Logic")
	void StartGameLevel(FName LevelName = "Map_Level1");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
};

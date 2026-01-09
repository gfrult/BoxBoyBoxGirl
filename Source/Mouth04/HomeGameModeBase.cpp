// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeGameModeBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Players/ABoxBot.h"


void AHomeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//UKismetSystemLibrary::PrintString(this->GetWorld(),TEXT("111"),true,true,FLinearColor(1,0.3,1),10);
	
}

AHomeGameModeBase::AHomeGameModeBase()
{
//
	DefaultPawnClass=AABoxBot::StaticClass();
}


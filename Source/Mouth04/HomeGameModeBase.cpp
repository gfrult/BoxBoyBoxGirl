// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeGameModeBase.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Players/ABoxBot.h"
#include "UI/MyHUD.h"


void AHomeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//UKismetSystemLibrary::PrintString(this->GetWorld(),TEXT("111"),true,true,FLinearColor(1,0.3,1),10);
	
}

AHomeGameModeBase::AHomeGameModeBase()
{
	//设置默认pawn类为ABoxBot
	DefaultPawnClass=AABoxBot::StaticClass();
	//将游戏模式中的HUD设置为我们c++中写的MyHUD类,ue中基于MyHUD实例化了一个类叫BP_HUD,此处给的路径指向BP_HUD
	static ConstructorHelpers::FClassFinder<AMyHUD> MyHUDPath(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/UMG/BP_HUD.BP_HUD_C'"));
	if(MyHUDPath.Class)
	{
		HUDClass= MyHUDPath.Class;
	}
	
}


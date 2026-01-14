// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Players/ABoxBot.h"
#include "Players/MyPlayerController.h"
#include "Actors/SharedCamera.h"
#include "UI/MyHUD.h"


void AHomeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//UKismetSystemLibrary::PrintString(this->GetWorld(),TEXT("111"),true,true,FLinearColor(1,0.3,1),10);
	UGameplayStatics::CreatePlayer(this, 1, true);
	
	FVector P1_Loc = FVector(0, 0, 100);
	FVector P2_Loc = FVector(200, 0, 100);
	
	AABoxBot* Bot1 = nullptr;
	if (Player1Class) // 检查一下你在编辑器里填没填，没填会崩
	{
		Bot1 = GetWorld()->SpawnActor<AABoxBot>(Player1Class, P1_Loc, FRotator::ZeroRotator);
	}

	// 生成 P2 (用 Player2Class)
	AABoxBot* Bot2 = nullptr;
	if (Player2Class)
	{
		Bot2 = GetWorld()->SpawnActor<AABoxBot>(Player2Class, P2_Loc, FRotator::ZeroRotator);
	}
	
	APlayerController* PC1 = UGameplayStatics::GetPlayerController(this, 0);
	if (PC1 && Bot1)
	{
		PC1->Possess(Bot1); 
	}
	
	APlayerController* PC2 = UGameplayStatics::GetPlayerController(this, 1);
	if (PC2 && Bot2)
	{
		PC2->Possess(Bot2); 
	}
	ASharedCamera* MyCamera = GetWorld()->SpawnActor<ASharedCamera>(ASharedCamera::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);

	if (PC1 && MyCamera)
	{
		PC1->SetViewTargetWithBlend(MyCamera, 0.0f);
	}
}

AHomeGameModeBase::AHomeGameModeBase()
{
	//设置默认pawn类为ABoxBot
	DefaultPawnClass=nullptr;
	//将游戏模式中的HUD设置为我们c++中写的MyHUD类,ue中基于MyHUD实例化了一个类叫BP_HUD,此处给的路径指向BP_HUD
	static ConstructorHelpers::FClassFinder<AMyHUD> MyHUDPath(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/UMG/BP_HUD.BP_HUD_C'"));
	if(MyHUDPath.Class)
	{
		HUDClass= MyHUDPath.Class;
	}
	PlayerControllerClass=AMyPlayerController::StaticClass();
}


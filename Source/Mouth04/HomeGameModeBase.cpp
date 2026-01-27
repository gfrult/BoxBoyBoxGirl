// Fill out your copyright notice in the Description page of Project Settings.


#include "HomeGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Players/ABoxBot.h"
#include "Players/MyPlayerController.h"
#include "Actors/SharedCamera.h"
#include "UI/MyHUD.h"
#include "GameFramework/PlayerStart.h"
#include "Public/GameInstance/MyGameInstance.h"


void AHomeGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	//UKismetSystemLibrary::PrintString(this->GetWorld(),TEXT("111"),true,true,FLinearColor(1,0.3,1),10);
	bool bIsTwoPlayer = false;
    TSubclassOf<AABoxBot> ClassP1 = Player1Class; // 默认值 (防止GI里没存)
    TSubclassOf<AABoxBot> ClassP2 = Player2Class; // 默认值

	FVector P1_Loc = FVector(0, 0, 200);
	FVector P2_Loc = FVector(100, 0, 200);
	
	TArray<AActor*> FoundStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), FoundStarts);

	for (AActor* StartActor : FoundStarts)
	{
		if (StartActor)
		{
			if (StartActor->ActorHasTag(TEXT("P1_Start")))
			{
				P1_Loc = StartActor->GetActorLocation();
			}
			else if (StartActor->ActorHasTag(TEXT("P2_Start")))
			{
				P2_Loc = StartActor->GetActorLocation();
			}
		}
	}
	
    UMyGameInstance* GI = Cast<UMyGameInstance>(GetGameInstance());
    if (GI)
    {
        // 从 GI 获取配置
        bIsTwoPlayer = GI->bIsTwoPlayerMode;
        
        // 如果 GI 里存了有效的类，就覆盖默认值
        if (GI->G_P1SelectedClass) ClassP1 = GI->G_P1SelectedClass;
        if (GI->G_P2SelectedClass) ClassP2 = GI->G_P2SelectedClass;
    }
	
    AABoxBot* Bot1 = nullptr;

    if (ClassP1)
    {
    	
        Bot1 = GetWorld()->SpawnActor<AABoxBot>(ClassP1, P1_Loc, FRotator::ZeroRotator);
    	Bot1->MaxBoxNumber=GI->G_P1MaxBoxNumber;
    	Bot1->RemainingBoxNumber=GI->G_P1MaxBoxNumber;
    	Bot1->bCanRetract=GI->bIsRetractMode;
    }

    APlayerController* PC1 = UGameplayStatics::GetPlayerController(this, 0);
    if (PC1 && Bot1)
    {
        PC1->Possess(Bot1);
    	FInputModeGameAndUI InputModeData;
    	PC1->SetInputMode(InputModeData);
    }

    AABoxBot* Bot2 = nullptr;

    if (bIsTwoPlayer) 
    {
        // 只有双人模式才创建第2个控制器
    	if (!UGameplayStatics::GetPlayerController(this, 1))
    	{
    		UGameplayStatics::CreatePlayer(this, 1, true);
    	}
    	
        if (ClassP2)
        {
        	
            Bot2 = GetWorld()->SpawnActor<AABoxBot>(ClassP2, P2_Loc, FRotator::ZeroRotator);
        	Bot2->MaxBoxNumber=GI->G_P2MaxBoxNumber;
        	Bot2->RemainingBoxNumber=GI->G_P2MaxBoxNumber;
        	Bot2->bCanRetract=GI->bIsRetractMode;
        }

        APlayerController* PC2 = UGameplayStatics::GetPlayerController(this, 1);
        if (PC2 && Bot2)
        {
            PC2->Possess(Bot2);
        }
    }
	
    // 4. 设置共享相机
    ASharedCamera* MyCamera = GetWorld()->SpawnActor<ASharedCamera>(ASharedCamera::StaticClass(), P1_Loc, FRotator::ZeroRotator);

    if (MyCamera)
    {
        // 设置 P1 视角
        if (PC1) PC1->SetViewTargetWithBlend(MyCamera, 0.0f);
        
        if (bIsTwoPlayer)
        {
            APlayerController* PC2 = UGameplayStatics::GetPlayerController(this, 1);
            if (PC2) PC2->SetViewTargetWithBlend(MyCamera, 0.0f);
        }

 
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


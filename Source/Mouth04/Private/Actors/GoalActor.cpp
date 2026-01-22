// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GoalActor.h"

#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Players/ABoxBot.h"


// Sets default values
AGoalActor::AGoalActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent=CreateDefaultSubobject<USceneComponent>("RootComponent");
	GoalDoorPaperFlipbookComponent=CreateDefaultSubobject<UPaperFlipbookComponent>("GoalDoorPaperFlipbookComponent");
	GoalDoorBox=CreateDefaultSubobject<UBoxComponent>("GoalDoorBox");
	
	GoalDoorPaperFlipbookComponent->SetupAttachment(RootComponent);
	GoalDoorBox->SetupAttachment(RootComponent);
	
	GoalDoorBox->SetBoxExtent(FVector(140,100,128));
	GoalDoorBox->SetCollisionProfileName(TEXT("Trigger"));
	GoalDoorBox->SetCollisionResponseToChannel(COLLISION_P1, ECR_Overlap);
	GoalDoorBox->SetCollisionResponseToChannel(COLLISION_P2, ECR_Overlap);
	
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook>GoalDoorPF(TEXT("/Script/Paper2D.PaperFlipbook'/Game/MyBoxGame/Textures/Goal/GoalDoor.GoalDoor'"));
	if (GoalDoorPF.Object)
	{
		GoalDoorPaperFlipbook=GoalDoorPF.Object;
	}

}

// Called when the game starts or when spawned
void AGoalActor::BeginPlay()
{
	Super::BeginPlay();
	
	GoalDoorPaperFlipbookComponent->SetFlipbook(GoalDoorPaperFlipbook);
	GoalDoorPaperFlipbookComponent->SetLooping(false);
	GoalDoorPaperFlipbookComponent->Stop();
	GoalDoorBox->OnComponentBeginOverlap.AddDynamic(this,&AGoalActor::OnOverlapBegin);
	GoalDoorBox->OnComponentEndOverlap.AddDynamic(this,&AGoalActor::OnOverlapEnd);
	
	UMyGameInstance* GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (GI)
	{
		TotalPlayersNeeded = GI->bIsTwoPlayerMode ? 2 : 1;
	}
	else
	{
		TotalPlayersNeeded=1;
	}
}

// Called every frame
void AGoalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGoalActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AABoxBot* Bot=Cast<AABoxBot>(OtherActor);
	if (Bot)
	{
		OverlappingPlayers.AddUnique(Bot);
		Bot->GoalDoor=this;
	}
	if (OverlappingPlayers.Num()>=TotalPlayersNeeded)
	{
		GoalDoorPaperFlipbookComponent->Play();
		for (AABoxBot*Actor:OverlappingPlayers)
		{
			Actor->UpArrowCom->SetVisibility(true);
		}
		
	}
	
}

void AGoalActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AABoxBot* Bot=Cast<AABoxBot>(OtherActor);
	if (Bot)
	{
		for (AABoxBot*Actor:OverlappingPlayers)
		{
			Actor->UpArrowCom->SetVisibility(false);
		}
		OverlappingPlayers.Remove(Bot);
		Bot->GoalDoor=nullptr;
		GoalDoorPaperFlipbookComponent->Reverse();
	}
	
	
}

void AGoalActor::TryToFinishLevel()
{
	if (OverlappingPlayers.Num()>=TotalPlayersNeeded)
	{
		UE_LOG(LogTemp, Log, TEXT("通关"));
		UMyGameInstance* GI = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
		if (GI)
		{
			int32 TotalStarsInLevel = 0;
			for (AABoxBot* Bot : OverlappingPlayers)
			{
				if (Bot)
				{
					TotalStarsInLevel += Bot->PlayerStarNum;
				}
			}
			UE_LOG(LogTemp, Log, TEXT("%d"), TotalStarsInLevel);
			FName LevelName=FName(*GetWorld()->GetName());
			GI->UpdateLevelProgress(LevelName,TotalStarsInLevel);
			GI->G_WidgetChose = EG_Widget::ChoseMap;
			UGameplayStatics::OpenLevel(this,FName("this"));
		}
	}
}


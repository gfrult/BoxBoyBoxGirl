// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Door.h"

#include "PaperSpriteComponent.h"
#include "Actors/ActionButton.h"
#include "Actors/BoxActor.h"
#include "Components/BoxComponent.h"
#include "Players/ABoxBot.h"


// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent=CreateDefaultSubobject<USceneComponent>("RootComponent");
	DoorSpriteComponent=CreateDefaultSubobject<UPaperSpriteComponent>("DoorSpriteComponent");
	DoorComponent=CreateDefaultSubobject<UBoxComponent>("DoorComponent");
	SafetyBox=CreateDefaultSubobject<UBoxComponent>("SafetyBox");
	
	DoorSpriteComponent->SetupAttachment(RootComponent);
	DoorComponent->SetupAttachment(DoorSpriteComponent);
	SafetyBox->SetupAttachment(DoorSpriteComponent);
	
	DoorComponent->SetBoxExtent(FVector(32.f, 32.f, 160.f));
	SafetyBox->SetBoxExtent(FVector(31.f, 31.f, 165.f));
	
	
	DoorComponent->SetCollisionProfileName("BlockAll");
	SafetyBox->SetCollisionProfileName("Trigger");
	SafetyBox->SetCollisionResponseToChannels(ECR_Overlap);
	
	
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	DoorSpriteComponent->SetSprite(DoorSprite);
	InitialLocation=DoorSpriteComponent->GetRelativeLocation();
	TargetLocation=InitialLocation;
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DoorButtons.Num()>0)
	{
		bool AllOpenDoor=true;
		for (AActionButton*Actor:DoorButtons)
		{
			if (!Actor->bIsPressed)
			{
				AllOpenDoor=false;
				break;
			}
		}
		UE_LOG(LogTemp, Log, TEXT("%hhd"),AllOpenDoor);
		if (AllOpenDoor)
		{
			OpenDoor();
		}
		if (!AllOpenDoor)
		{
			CloseDoor();
			TArray<AActor*>OverlappedActors;
			SafetyBox->GetOverlappingActors(OverlappedActors);
			for (AActor* OverlappedActor:OverlappedActors)
			{
				if (OverlappedActor->IsA<AABoxBot>()||OverlappedActor->IsA<ABoxActor>())
				{
					return;
				}
			}
		}
	}
	FVector CurrentLoc=DoorSpriteComponent->GetRelativeLocation();
	if (!CurrentLoc.Equals(TargetLocation, 0.1f))
	{
		FVector NewLocation=FMath::VInterpTo(CurrentLoc,TargetLocation,DeltaTime,MoveSpeed);
		DoorSpriteComponent->SetRelativeLocation(NewLocation);
	}
}

void ADoor::OpenDoor()
{
	TargetLocation=InitialLocation+MoveOffset;
}

void ADoor::CloseDoor()
{
	TargetLocation=InitialLocation;
}


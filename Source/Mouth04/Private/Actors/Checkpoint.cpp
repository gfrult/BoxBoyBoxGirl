// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Checkpoint.h"
#include "Players/ABoxBot.h"
#include "Components/BoxComponent.h"


// Sets default values
ACheckpoint::ACheckpoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CheckPoint=CreateDefaultSubobject<UBoxComponent>("CheckPoint");
	RootComponent=CheckPoint;
	
	CheckPoint->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	CheckPoint->SetCollisionProfileName(TEXT("Trigger"));
	CheckPoint->SetCollisionResponseToChannel(COLLISION_P1, ECR_Overlap);
	CheckPoint->SetCollisionResponseToChannel(COLLISION_P2, ECR_Overlap);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	if (CheckPoint)
	{
		CheckPoint->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
	}
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)UE_LOG(LogTemp, Log, TEXT("碰撞"));
	AABoxBot* Bot = Cast<AABoxBot>(OtherActor);
	if (Bot)
	{
		Bot->RespawnLocation = GetActorLocation();
		UE_LOG(LogTemp, Log, TEXT("存档"));
	}
}


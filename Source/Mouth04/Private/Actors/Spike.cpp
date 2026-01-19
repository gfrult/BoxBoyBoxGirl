// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Spike.h"

#include "PaperSpriteComponent.h"
#include "Players/ABoxBot.h"


// Sets default values
ASpike::ASpike()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Spike=CreateDefaultSubobject<UPaperSpriteComponent>("Spike");
	
	RootComponent = Spike;
	
	Spike->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void ASpike::BeginPlay()
{
	Super::BeginPlay();
	if (Spike)
	{
		Spike->OnComponentHit.AddDynamic(this,&ASpike::OnTrapHit);
	}
}

// Called every frame
void ASpike::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpike::OnTrapHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor||OtherActor==this)return;
	AABoxBot* Actor=Cast<AABoxBot>(OtherActor);
	
	if (Actor)
	{
		Actor->Respawn();
	}
}


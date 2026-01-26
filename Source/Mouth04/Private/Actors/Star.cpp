// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Star.h"
#include "Players/ABoxBot.h"
#include "PaperSpriteComponent.h"
#include "Components/SphereComponent.h"
#include "PaperSprite.h"


// Sets default values
AStar::AStar()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent=CreateDefaultSubobject<USceneComponent>("RootSceneComponent");
	StarSpriteCom=CreateDefaultSubobject<UPaperSpriteComponent>("StarSpriteComponent");
	StarBox=CreateDefaultSubobject<USphereComponent>("StarBox");
	
	StarSpriteCom->SetupAttachment(RootComponent);
	StarBox->SetupAttachment(RootComponent);
	
	StarBox->SetSphereRadius(32);
	StarBox->SetCollisionProfileName(TEXT("Trigger"));
	StarBox->SetCollisionResponseToChannel(COLLISION_P1, ECR_Overlap);
	StarBox->SetCollisionResponseToChannel(COLLISION_P2, ECR_Overlap);
	
	static ConstructorHelpers::FObjectFinder<UPaperSprite>StarPF(TEXT("/Script/Paper2D.PaperSprite'/Game/MyBoxGame/Textures/Star/Star_Sprite.Star_Sprite'"));
	if (StarPF.Object)
	{
		StarSpriteCom->SetSprite(StarPF.Object);
	}
}

// Called when the game starts or when spawned
void AStar::BeginPlay()
{
	Super::BeginPlay();
	StarBox->OnComponentBeginOverlap.AddDynamic(this,&AStar::OnOverlapBegin);
}

// Called every frame
void AStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorLocalRotation(FRotator(0, 90 * DeltaTime, 0));
}

void AStar::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AABoxBot* Bot = Cast<AABoxBot>(OtherActor);
	UE_LOG(LogTemp, Log, TEXT("%s,%s"),*OtherActor->GetName(),*OtherComp ->GetName());
	if (Bot)
	{
		if (OtherComp==Bot->GetRootComponent())
		{
			float Distance = FVector::Dist(Bot->GetActorLocation(), GetActorLocation());
			
			if (Distance > 80.0f) 
			{
				UE_LOG(LogTemp, Warning, TEXT("距离太远"));
				return; 
			}
			UE_LOG(LogTemp, Log, TEXT("%s,%s"),*OtherActor->GetName(),*OtherComp ->GetName());
			if (!Bot->PlayerStarNum)
			{
				Bot->PlayerStarNum++;
				UE_LOG(LogTemp, Log, TEXT("获得星星"));
				Destroy();
			}
		}
	}
	
}


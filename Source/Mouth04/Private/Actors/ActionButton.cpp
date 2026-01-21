// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ActionButton.h"

#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Players/ABoxBot.h"
#include "Actors/BoxActor.h"
#include "DataWrappers/ChaosVDQueryDataWrappers.h"


// Sets default values
AActionButton::AActionButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent=CreateDefaultSubobject<USceneComponent>("Root");
	ButtonPaperSpriteComponent=CreateDefaultSubobject<UPaperSpriteComponent>("ButtonPaperSpriteComponent");
	ButtonBox=CreateDefaultSubobject<UBoxComponent>("ButtonBox");
	BaseBox=CreateDefaultSubobject<UBoxComponent>("BaseBox");
	
	BaseBox->SetupAttachment(RootComponent);
	ButtonBox->SetupAttachment(RootComponent);
	ButtonPaperSpriteComponent->SetupAttachment(RootComponent);
	
	ButtonBox->SetCollisionProfileName("Trigger");
	ButtonBox->SetCollisionResponseToChannels(ECR_Overlap);
	BaseBox->SetCollisionProfileName("BlockAllDynamic");
	ButtonPaperSpriteComponent->SetCollisionProfileName("NoCollision");
	
	ButtonBox->SetRelativeLocation(FVector(-0.7f,0.0f,6.5f));
	ButtonBox->SetRelativeScale3D(FVector(0.885f,1.0f,0.1475f));
	BaseBox->SetRelativeLocation(FVector(-0.9f,0.0f,-4.2f));
	BaseBox->SetRelativeScale3D(FVector(1.0f,1.0f,0.185f));
	ButtonPaperSpriteComponent->SetRelativeLocation(FVector(0.0f,0.0f,-12.0f));
	
	
}

// Called when the game starts or when spawned
void AActionButton::BeginPlay()
{
	Super::BeginPlay();
	ButtonPaperSpriteComponent->SetSprite(ButtonUnPressed);
	
	ButtonBox->OnComponentBeginOverlap.AddDynamic(this,&AActionButton::OnOverlapBegin);
	ButtonBox->OnComponentEndOverlap.AddDynamic(this,&AActionButton::OnOverlapEnd);
}

// Called every frame
void AActionButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AActionButton::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("%s"),*OtherActor->GetName());
	CheckOverlap();
}

void AActionButton::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("%s"),*OtherActor->GetName());
	CheckOverlap();
}

void AActionButton::CheckOverlap()
{
	if (bIsProcessing)return;
	bIsProcessing=true;
	TArray<AActor*> OverlappedActors;
	ButtonBox->GetOverlappingActors(OverlappedActors);
	for (AActor* Actor : OverlappedActors)
	{
		UE_LOG(LogTemp, Error, TEXT("%s"),*Actor->GetName());
	}
	bool bShouldBePressed=false;
	for (AActor* Actor : OverlappedActors)
	{
		if (Actor->IsA<AABoxBot>()||Actor->IsA<ABoxActor>())
		{
			bShouldBePressed=true;
			break;
		}
	}
	if (bIsPressed != bShouldBePressed)
	{
		bIsPressed = bShouldBePressed;
		if (bShouldBePressed)
        	{
        		ButtonPaperSpriteComponent->SetSprite(ButtonPressed);
        		
        	}
        else
        	{
        		ButtonPaperSpriteComponent->SetSprite(ButtonUnPressed);
        		
        	}
		
	}
	bIsProcessing=false;
}


	



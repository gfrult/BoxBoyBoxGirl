// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/BoxActor.h"

#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Components/SphereComponent.h"


// Sets default values
ABoxActor::ABoxActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Box=CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent=Box;
	SpriteComponent=CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComponent"));
	BotPhysMat = CreateDefaultSubobject<UPhysicalMaterial>(TEXT("BotPhysMat"));
	Wheel=CreateDefaultSubobject<USphereComponent>(TEXT("Wheel"));
	
	SpriteComponent->SetupAttachment(RootComponent);
	Wheel->SetupAttachment(RootComponent);
	
	BotPhysMat->Restitution = 0.0f;  
	BotPhysMat->Friction = 0.0f;     
	BotPhysMat->Density = 1.0f;
	BotPhysMat->FrictionCombineMode=EFrictionCombineMode::Min;
	BotPhysMat->RestitutionCombineMode=EFrictionCombineMode::Min;
	
	Box->SetBoxExtent(FVector(31,31,31));
	Box->SetPhysMaterialOverride(BotPhysMat);
	Box->SetCollisionProfileName(TEXT("BlockAll"));
	Box->GetBodyInstance()->bLockYTranslation = true;
	Box->SetMassOverrideInKg(NAME_None, 0.0f, true);
	Box->SetSimulatePhysics(false);
	
	Wheel->SetSphereRadius(1.0f);
	Wheel->SetRelativeLocation(FVector(0.f, 0.f, -31.0f));
	Wheel->SetPhysMaterialOverride(BotPhysMat);
	Wheel->SetCollisionProfileName(TEXT("BlockAll"));
	Wheel->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Wheel->SetMassOverrideInKg(NAME_None, 0.0f, true);
	Wheel->SetSimulatePhysics(false);
	
}

// Called when the game starts or when spawned
void ABoxActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


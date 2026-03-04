// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/SlimeSurface.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"

// Sets default values
ASlimeSurface::ASlimeSurface()
{
	PrimaryActorTick.bCanEverTick = false; 
	
	SlimeBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SlimeBox"));
	RootComponent = SlimeBox;
	
	SlimeSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SlimeSprite"));
	SlimeSprite->SetupAttachment(RootComponent);
	
	SlimeBox->SetCollisionProfileName(TEXT("BlockAll"));
	SlimeSprite->SetCollisionProfileName(TEXT("NoCollision"));
	
	SlimeSprite->SetCastShadow(true);
}
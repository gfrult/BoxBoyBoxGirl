// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/SharedCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ASharedCamera::ASharedCamera()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootScene=CreateDefaultSubobject<USceneComponent>("RootScene");
	SpringArm=CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraComp=CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	
	RootComponent=RootScene;
	SpringArm->SetupAttachment(RootComponent);
	CameraComp->SetupAttachment(SpringArm);
	
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetRelativeRotation(FRotator(0,-90,0));
	SpringArm->TargetArmLength = 4500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 0.8f; 
	SpringArm->CameraLagMaxDistance = 400.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 200.0f);
	
	CameraComp->FieldOfView = 15.0f;
	
	
}

// Called when the game starts or when spawned
void ASharedCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASharedCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	APawn* P1 = UGameplayStatics::GetPlayerPawn(this, 0);
	APawn* P2 = UGameplayStatics::GetPlayerPawn(this, 1);
	
	FVector TargetCenter;
	float PlayersDistance = 0.0f;
	if (P1 && P2)
	{
		TargetCenter = (P1->GetActorLocation() + P2->GetActorLocation()) / 2.0f;
		PlayersDistance = FVector::Dist(P1->GetActorLocation(), P2->GetActorLocation());
	}
	else if (P1)
	{
		TargetCenter = P1->GetActorLocation();
		PlayersDistance = 0.0f;
	}
	else if (P2)
	{
		TargetCenter = P2->GetActorLocation();
		PlayersDistance = 0.0f;
	}
	else return;
    
	SetActorLocation(TargetCenter);
	
	float DesiredLength = MinArmLength + (PlayersDistance * ZoomMultiplier);
	
	float ClampedLength = FMath::Clamp(DesiredLength, MinArmLength, MaxArmLength);
	
	float SmoothLength = FMath::FInterpTo(SpringArm->TargetArmLength, ClampedLength, DeltaTime, ZoomInterpSpeed);
	
	SpringArm->TargetArmLength = SmoothLength;
}


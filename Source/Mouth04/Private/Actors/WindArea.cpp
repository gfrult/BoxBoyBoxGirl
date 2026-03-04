// Fill out your copyright notice in the Description page of Project Settings.

#include "Actors/WindArea.h"
#include "Components/BoxComponent.h"
#include "Players/ABoxBot.h"
#include "TimerManager.h"
#include "NiagaraComponent.h"
#include "Actors/BoxActor.h"
#include "Engine/Engine.h"
// Sets default values
AWindArea::AWindArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WindBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WindBox"));
	RootComponent = WindBox;

	// 设置默认大小
	WindBox->SetBoxExtent(FVector(300.0f, 300.0f, 300.0f));

	// 碰撞设置
	WindBox->SetCollisionProfileName(TEXT("Trigger"));
	WindBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	WindBox->SetCollisionResponseToChannel(COLLISION_P1, ECR_Overlap);
	WindBox->SetCollisionResponseToChannel(COLLISION_P2, ECR_Overlap);
	WindBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	WindBox->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	WindBox->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	
	SnowParticleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SnowParticleComponent"));
	SnowParticleComponent->SetupAttachment(RootComponent);
	
	bIsWindActive = false;
}

// Called when the game starts or when spawned
void AWindArea::BeginPlay()
{
	Super::BeginPlay();
	
	// 绑定重叠事件
	WindBox->OnComponentBeginOverlap.AddDynamic(this, &AWindArea::OnOverlapBegin);
	WindBox->OnComponentEndOverlap.AddDynamic(this, &AWindArea::OnOverlapEnd);

	TArray<AActor*> InitialOverlappingActors;
	WindBox->GetOverlappingActors(InitialOverlappingActors);
	
	for (AActor* Actor : InitialOverlappingActors)
	{
		if (AABoxBot* Bot = Cast<AABoxBot>(Actor))
		{
			OverlappingBots.Add(Bot);
		}
		else if (ABoxActor* Box = Cast<ABoxActor>(Actor))
		{
			OverlappingBoxes.Add(Box);
		}
	}
	
	if (SnowParticleComponent)
	{
		// 传给粒子的生成范围
		SnowParticleComponent->SetVectorParameter(TEXT("User.BoxSize"), WindBox->GetUnscaledBoxExtent() * 2.0f);
	}
	// 启动风区循环计时器
	bIsWindActive = false;
	ToggleWind();
}

// Called every frame
void AWindArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsWindActive)
	{
		for (ABoxActor* BoxActor : OverlappingBoxes)
		{
			bool bIsSimulating = BoxActor->Box->IsSimulatingPhysics();
			if (bIsSimulating)
				{
					FVector WindForce = FVector(WindStrength * 15.0f, 0.0f, 0.0f);
					BoxActor->Box->AddForce(WindForce, NAME_None, true);
				}
		}
	}
}

void AWindArea::ToggleWind()
{
	// 翻转风的状态
	bIsWindActive = !bIsWindActive;

	// 更新所有当前在风区内的玩家的风力
	for (AABoxBot* Bot : OverlappingBots)
	{
		if (IsValid(Bot))
		{
			Bot->WindVelocityX = bIsWindActive ? WindStrength : 0.0f;
		}
	}
	if (SnowParticleComponent)
	{
		float TargetWind = bIsWindActive ? WindStrength : 0.0f;
		SnowParticleComponent->SetFloatParameter(TEXT("User.WindSpeedX"), TargetWind);
	}
	float NextDelay = bIsWindActive ? BlowDuration : StopDuration;
	GetWorld()->GetTimerManager().SetTimer(WindTimerHandle, this, &AWindArea::ToggleWind, NextDelay, false);
}

void AWindArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AABoxBot* Bot = Cast<AABoxBot>(OtherActor);
	if (Bot)
	{
		OverlappingBots.Add(Bot);
		
		if (bIsWindActive)
		{
			Bot->WindVelocityX = WindStrength;
		}
	}
	ABoxActor* Box = Cast<ABoxActor>(OtherActor);
	if (Box)
	{
		OverlappingBoxes.Add(Box);
	}
}

void AWindArea::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AABoxBot* Bot = Cast<AABoxBot>(OtherActor);
	if (Bot)
	{
		OverlappingBots.Remove(Bot);
		
		Bot->WindVelocityX = 0.0f;
	}
	ABoxActor* Box = Cast<ABoxActor>(OtherActor);
	if (Box)
	{
		OverlappingBoxes.Remove(Box);
	}
}
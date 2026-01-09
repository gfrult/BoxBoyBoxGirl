// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/ABoxBot.h"

#include "MovieSceneSequenceID.h"
#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PaperFlipbook.h"
#include "Components/SphereComponent.h"
#include "Actors/BoxActor.h"
#include "Animation/AnimInstanceProxy.h"
#include "DataWrappers/ChaosVDQueryDataWrappers.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

// Sets default values
AABoxBot::AABoxBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxBody=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxFoot=CreateDefaultSubobject<USphereComponent>(TEXT("BoxFoot"));
	RootComponent = BoxBody;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FootFlipbookComponent=CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("FlipbookComponent"));
	BotPhysMat = CreateDefaultSubobject<UPhysicalMaterial>(TEXT("BotPhysMat"));
	BodySpriteComponent=CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BodySpriteComponent"));
	EyesFlipbookComponent=CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EyesFlipbookComponent"));
	
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	BodySpriteComponent->SetupAttachment(BoxBody);
	BoxFoot->SetupAttachment(BoxBody);
	FootFlipbookComponent->SetupAttachment(BoxFoot);
	EyesFlipbookComponent->SetupAttachment(BoxBody);
	
	BotPhysMat->Restitution = 0.0f;  
	BotPhysMat->Friction = 0.0f;     
	BotPhysMat->Density = 1.0f;
	BotPhysMat->FrictionCombineMode=EFrictionCombineMode::Min;
	BotPhysMat->RestitutionCombineMode=EFrictionCombineMode::Min;
	
	BoxBody->SetPhysMaterialOverride(BotPhysMat);
	BoxBody->SetBoxExtent(FVector(32.0f, 32.0f, 32.0f)); 
	BoxBody->SetSimulatePhysics(true);
	BoxBody->GetBodyInstance()->bLockXRotation = true;
	BoxBody->GetBodyInstance()->bLockYRotation = true;
	BoxBody->GetBodyInstance()->bLockZRotation = true;
	BoxBody->SetCollisionProfileName(TEXT("Pawn")); 
	BoxBody->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
	
	BoxFoot->SetPhysMaterialOverride(BotPhysMat);
	BoxFoot->SetSphereRadius(32.0f);
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, -12.0f));
	BoxFoot->SetCollisionProfileName(TEXT("Pawn")); 
	BoxFoot->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	BoxFoot->SetMassOverrideInKg(NAME_None, 0.0f, true);
	
	SpringArm->SetRelativeRotation(FRotator(0,-90,0));
	SpringArm->TargetArmLength = 4500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 0.5f;
	SpringArm->CameraLagMaxDistance = 350.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 200.0f);
	
	Camera->FieldOfView = 15.0f;
	
	static ConstructorHelpers::FObjectFinder<UPaperSprite> BodySprite(TEXT("/Script/Paper2D.PaperSprite'/Game/MyBoxGame/Textures/Sheep/BaseSprite/SheepBodyBase_Sprite.SheepBodyBase_Sprite'"));
	if (BodySprite.Object)
	{
		BodySpriteComponent->SetSprite(BodySprite.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> RunPaperFlipbook(TEXT("/Script/Paper2D.PaperFlipbook'/Game/MyBoxGame/Textures/Sheep/PaperFilpbook/PF_SheepFootRun.PF_SheepFootRun'"));
	if (RunPaperFlipbook.Object)
	{
		FootPaperFlipbook=RunPaperFlipbook.Object;
		//FootFlipbookComponent->SetFlipbook(FootPaperFlipbook.Object);
		//FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,5));
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> EyesPaperFlipbook(TEXT("/Script/Paper2D.PaperFlipbook'/Game/MyBoxGame/Textures/Sheep/PaperFilpbook/PF_SheepFace_zhayan.PF_SheepFace_zhayan'"));
	if (EyesPaperFlipbook.Object)
	{
		EyesFlipbookComponent->SetFlipbook(EyesPaperFlipbook.Object);
		EyesFlipbookComponent->SetRelativeLocation(FVector(0,5,0));
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> StandRF(TEXT("/Script/Paper2D.PaperFlipbook'/Game/MyBoxGame/Textures/Sheep/PaperFilpbook/PF_SheepFoot_Stand.PF_SheepFoot_Stand'"));
	if (StandRF.Object)
	{
		StandPaperFlipbook=StandRF.Object;
		FootFlipbookComponent->SetFlipbook(StandRF.Object);
		FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,10));
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> JumpPF(TEXT("/Script/Paper2D.PaperFlipbook'/Game/MyBoxGame/Textures/Sheep/PaperFilpbook/PF_SheepFootJump.PF_SheepFootJump'"));
	if (JumpPF.Object)
	{
		JumpPaperFlipbook=JumpPF.Object;
	}
	
	IsSpawnMode=false;
	ClockSpawnLeft=false;
	ClockSpawnRight=false;
	IsInAir=false;
	IsPutDown=false;
}

// Called when the game starts or when spawned
void AABoxBot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABoxBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CheckIsOnGround())
	{
		IsInAir=false;
	}
	else
	{
		IsInAir=true;
		FootFlipbookComponent->SetFlipbook(JumpPaperFlipbook);
	}
}

// Called to bind functionality to input
void AABoxBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Right"),this,&AABoxBot::RightFunction);
	PlayerInputComponent->BindAction(TEXT("Jump"),IE_Pressed,this,&AABoxBot::JumpFunction);
	PlayerInputComponent->BindAction(TEXT("Spawn"),IE_Pressed,this,&AABoxBot::BeginSpawnBox);
	PlayerInputComponent->BindAction(TEXT("Spawn"),IE_Released,this,&AABoxBot::EndSpawnBox);
	PlayerInputComponent->BindAction("SpawnUp", IE_Pressed, this, &AABoxBot::OnSpawnUp);
	PlayerInputComponent->BindAction("Put Down", IE_Pressed, this, &AABoxBot::BeginPutDownBox);
	PlayerInputComponent->BindAction(TEXT("Put Down"),IE_Released,this,&AABoxBot::EndPutDownBox);
}

void AABoxBot::RightFunction(float AxisValue)
{
	if (AxisValue==0)
	{
		FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,10));
		FVector CurrentVel = BoxBody->GetPhysicsLinearVelocity();
		BoxBody->SetPhysicsLinearVelocity(FVector(0, 0, CurrentVel.Z));
		ClockSpawnLeft=false;
		ClockSpawnRight=false;
		if (IsInAir)return;
		FootFlipbookComponent->SetFlipbook(StandPaperFlipbook);
		return;
	}
	FVector CurrentVelocity = BoxBody->GetPhysicsLinearVelocity();
	if (IsSpawnMode)
	{
		BoxBody->SetPhysicsLinearVelocity(FVector(0,0,CurrentVelocity.Z));
		if (AxisValue>0)
		{
			if (!ClockSpawnRight)
			{
				OnSpawnRight();
				ClockSpawnLeft=false;
				ClockSpawnRight=true;
			}
			return;
		}
		if (AxisValue<0)
		{
			if (!ClockSpawnLeft)
			{
				OnSpawnLeft();
				ClockSpawnLeft=true;
				ClockSpawnRight=false;
			}
			return;
		}
		
	}
	if (IsPutDown)return;
	if (!IsInAir||!CheckIsHooked())
	{
		uint8 RotationYaw=0;
		if (AxisValue>0)
		{
			RotationYaw=180;
		}
		if (AxisValue<0)
		{
			RotationYaw=0;
		}
		EyesFlipbookComponent->SetRelativeRotation(FRotator(0,RotationYaw,0));
		FootFlipbookComponent->SetRelativeRotation(FRotator(0,RotationYaw,0));
		FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,6));
		
		//float DeltaTime = GetWorld()->GetDeltaSeconds();
		float Speed = 200;
		FVector DeltaLoc = FVector(AxisValue * Speed /** DeltaTime*/ , 0, 0);
		BoxBody->SetPhysicsLinearVelocity(FVector( DeltaLoc.X,0,CurrentVelocity.Z));
		//AddActorLocalOffset(DeltaLoc);
		if (IsInAir)return;
		FootFlipbookComponent->SetFlipbook(FootPaperFlipbook);
	}
	
}
void AABoxBot::JumpFunction()
{
	FVector CurrentVelocity = BoxBody->GetPhysicsLinearVelocity();
	if (IsSpawnMode)
	{
		BoxBody->SetPhysicsLinearVelocity(FVector(0,0,CurrentVelocity.Z));
		return;
	}
	if (IsPutDown)return;
	if (!IsInAir)
	{
		BoxBody->SetPhysicsLinearVelocity(FVector(CurrentVelocity.X,CurrentVelocity.Y,0));
		BoxBody->AddImpulse(FVector(0,0,25000));
		//BoxCollision->AddImpulse(GetActorUpVector().RotateAngleAxis(35.f,FVector::LeftVector)*25000.f);
	}
	
}

bool AABoxBot::CheckIsOnGround()
{
	FVector Start = BoxBody->GetComponentLocation();
	//UE_LOG(LogTemp, Log, TEXT("X: %f, Y: %f, Z: %f"), Start.X, Start.Y, Start.Z);
	FVector End = Start + (FVector::DownVector * 50.0f);
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);
	CollisionParameters.AddIgnoredActors(BoxChain);
	FHitResult HitResult;
	bool IsOnGround = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParameters);
	return IsOnGround;
}

void AABoxBot::SpawnBox(FVector Direction)
{
	if (!IsSpawnMode ) return;
	FVector SpawnLoc ;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
    if (BoxChain.Num()>0)
    {
    	SpawnLoc = BoxChain.Last()->GetActorLocation()+ (Direction * 64.0f);
    	FVector Start = BoxChain.Last()->GetActorLocation();
    	FVector End = Start + (Direction * 35.0f);
    	FHitResult HitResult;
    	FCollisionQueryParams CollisionParameters;
    	CollisionParameters.AddIgnoredActor(BoxChain.Last());
    	
    	bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility,CollisionParameters);
    	
    	if (IsHit)
    	{
    		UE_LOG(LogTemp, Log, TEXT("111111"));
    		if (BoxChain.Num()==1)
    		{
    			if (HitResult.GetComponent() == BoxBody)
    			{
				   
    				AActor* BoxToKill = BoxChain.Pop();
    				if (IsValid(BoxToKill))
    				{
    					BoxToKill->Destroy();
    				}
    			}
    		}
    		if (BoxChain.Num()>=2)
    		{
    			if (HitResult.GetActor() == BoxChain[BoxChain.Num()-2])
    			{
    				AActor* BoxToKill = BoxChain.Pop();
    				if (IsValid(BoxToKill))
    				{
    					BoxToKill->Destroy();
    				}
    				UE_LOG(LogTemp, Log, TEXT("%d"),BoxChain.Num());
    			}
    		}
    		return;
    	}
    }
    else
    {
    	if (Direction.Z<0)return;
    	SpawnLoc = GetActorLocation() + (Direction * 64.0f);
    	FVector Start = BoxBody->GetComponentLocation();
    	FVector End = Start + (Direction * 35.0f);
    	FHitResult HitResult;
    	FCollisionQueryParams CollisionParameters;
    	CollisionParameters.AddIgnoredActor(this);
    	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility,CollisionParameters))return;
    	
    }
	if (BoxChain.Num()>=MaxBoxNumber)return;
	ABoxActor* NewBox = GetWorld()->SpawnActor<ABoxActor>(ABoxActor::StaticClass(), SpawnLoc, FRotator::ZeroRotator, SpawnParams);
	if (NewBox)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);
		NewBox->AttachToComponent(GetRootComponent(), AttachmentRules);
		MoveIgnoreActorAdd(NewBox);
		NewBox->Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
		BoxChain.Add(NewBox);
		UE_LOG(LogTemp, Log, TEXT("%d"),BoxChain.Num());
		
	}
}

void AABoxBot::OnSpawnUp()
{
	SpawnBox(FVector(0, 0, 1));
}

void AABoxBot::OnSpawnRight()
{
	SpawnBox(FVector(1, 0, 0));
}

void AABoxBot::OnSpawnLeft()
{
	SpawnBox(FVector(-1, 0, 0));
}

void AABoxBot::OnSpawnDown()
{
	SpawnBox(FVector(0, 0, -1));
}

void AABoxBot::BeginPutDownBox()
{
	
	if (IsSpawnMode)
	{
		OnSpawnDown();
		return;
	}
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
	IsPutDown=true;
}

void AABoxBot::EndPutDownBox()
{
	if (IsSpawnMode)return;
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, -12.0f));
	IsPutDown=false;
}

bool AABoxBot::CheckIsHooked()
{
	if (BoxChain.Num()==0)return false;
	
	FHitResult HitResult;
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);
	CollisionParameters.AddIgnoredActors(BoxChain);
	for (AActor* Box : BoxChain)
	{
		if (!IsValid(Box)) continue;
		FVector Start = Box->GetActorLocation();
		FVector End = Start + FVector(0.0f, 0.0f, -35.0f);
		bool IsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility,CollisionParameters);
		if (IsHit)return true;
	}

	return false;
}


void AABoxBot::BeginSpawnBox()
{
	IsSpawnMode=true;
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
}

void AABoxBot::EndSpawnBox()
{
	IsSpawnMode=false;
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, -12.0f));
}


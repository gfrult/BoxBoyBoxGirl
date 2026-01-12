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
	
	BodySpriteInitialRelativeLoc = BodySpriteComponent->GetRelativeLocation();// 记录初始相对位置
	
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
	BoxBody->SetBoxExtent(FVector(31.0f, 31.0f, 31.0f)); 
	BoxBody->SetSimulatePhysics(true);
	BoxBody->GetBodyInstance()->bLockXRotation = true;
	BoxBody->GetBodyInstance()->bLockYRotation = true;
	BoxBody->GetBodyInstance()->bLockZRotation = true;
	BoxBody->SetCollisionProfileName(TEXT("Pawn")); 
	BoxBody->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	
	
	BoxFoot->SetPhysMaterialOverride(BotPhysMat);
	BoxFoot->SetSphereRadius(30.0f);
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, -12.0f));
	BoxFoot->SetCollisionProfileName(TEXT("Pawn")); 
	BoxFoot->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	BoxFoot->SetMassOverrideInKg(NAME_None, 0.0f, true);
	
	SpringArm->SetRelativeRotation(FRotator(0,-90,0));
	SpringArm->TargetArmLength = 4500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 0.8f; 
	SpringArm->CameraLagMaxDistance = 400.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 200.0f);
	
	Camera->FieldOfView = 15.0f;
	PlayerXVector=-1.0f;
	BoxYVector=0;
	
	static ConstructorHelpers::FObjectFinder<UPaperSprite> BodySprite(TEXT("/Script/Paper2D.PaperSprite'/Game/MyBoxGame/Textures/Sheep/BaseSprite/SheepBodyBase_Sprite.SheepBodyBase_Sprite'"));
	if (BodySprite.Object)
	{
		BodySpriteComponent->SetSprite(BodySprite.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperSprite> PutDownSprite(TEXT("/Script/Paper2D.PaperSprite'/Game/MyBoxGame/Textures/Sheep/Box/BoxSheepB_Sprite.BoxSheepB_Sprite'"));
	if (PutDownSprite.Object)
	{
		BoxSheepB=PutDownSprite.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> RunPF(TEXT("/Script/Paper2D.PaperFlipbook'/Game/MyBoxGame/Textures/Sheep/PaperFilpbook/PF_SheepFootRun.PF_SheepFootRun'"));
	if (RunPF.Object)
	{
		RunPaperFlipbook = RunPF.Object;
		//FootFlipbookComponent->SetFlipbook(FootPaperFlipbook.Object);
		//FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,5));
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> HookPF(TEXT("/Script/Paper2D.PaperFlipbook'/Game/MyBoxGame/Textures/Sheep/PaperFilpbook/SheepFoot_Suspended.SheepFoot_Suspended'"));
	if (HookPF.Object)
	{
		HookPaperFlipbook = HookPF.Object;
		//FootFlipbookComponent->SetFlipbook(FootPaperFlipbook.Object);
		//FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,5));
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> EyesPaperFlipbook(TEXT("/Script/Paper2D.PaperFlipbook'/Game/MyBoxGame/Textures/Sheep/PaperFilpbook/PF_SheepFace_zhayan.PF_SheepFace_zhayan'"));
	if (EyesPaperFlipbook.Object)
	{
		EyesFlipbook=EyesPaperFlipbook.Object;
		EyesFlipbookComponent->SetFlipbook(EyesFlipbook);
		EyesFlipbookComponent->SetRelativeLocation(FVector(0,5,0));
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperFlipbook> SpawnBoxEyesPaperFlipbook(TEXT("/Script/Paper2D.PaperFlipbook'/Game/MyBoxGame/Textures/Sheep/PaperFilpbook/SheepFace_Spawn.SheepFace_Spawn'"));
	if (SpawnBoxEyesPaperFlipbook.Object)
	{
		SpawnBoxEyesFlipbook=SpawnBoxEyesPaperFlipbook.Object;
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
	
	bIsSpawnMode=false;
	bClockSpawnLeft=false;
	bClockSpawnRight=false;
	bIsInAir=false;
	bIsPutDown=false;
	PrimaryActorTick.bCanEverTick = true;	// 开启Tick（必须，否则颤动逻辑不执行）
	bIsThrowing=false;
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
		bIsInAir=false;
	}
	else
	{
		bIsInAir=true;
		if (CheckIsHooked())
		{
			FootFlipbookComponent->SetFlipbook(HookPaperFlipbook);
		}
		else
		{
			FootFlipbookComponent->SetFlipbook(JumpPaperFlipbook);
		}
	}
	float ZVelocity = BoxBody->GetPhysicsLinearVelocity().Z;
	
	float TargetOffsetZ = 200.0f;

	if (ZVelocity < -200.0f) 
	{
		TargetOffsetZ = -100.0f; 
	}
	float CurrentOffsetZ = SpringArm->SocketOffset.Z;
	float NewOffsetZ = FMath::FInterpTo(CurrentOffsetZ, TargetOffsetZ, DeltaTime, 1.0f);
	SpringArm->SocketOffset = FVector(SpringArm->SocketOffset.X, SpringArm->SocketOffset.Y, NewOffsetZ);
	
	if (bIsSpawnMode && BodySpriteComponent)
	{
		// 计算颤动偏移：使用正弦函数实现左右往复运动
		float ShakeOffset = FMath::Sin(GetWorld()->GetTimeSeconds() * ShakeFrequency * 2 * PI) * ShakeAmplitude;
        
		// 构造新的相对位置：仅X轴左右偏移，Y/Z轴保持初始值（不上下/前后动）
		FVector NewRelativeLoc = BodySpriteInitialRelativeLoc;
		NewRelativeLoc.X = BodySpriteInitialRelativeLoc.X + ShakeOffset;
        
		// 设置新的相对位置（相对BoxBody，BoxBody本身不动）
		BodySpriteComponent->SetRelativeLocation(NewRelativeLoc);
		
		// 表情组件同步偏移（核心：跟随主体的X轴偏移）
		FVector EyeInitialLoc = EyesFlipbookComponent->GetRelativeLocation(); // 眼睛初始位置(0,5,0)
		FVector NewEyeRelativeLoc = EyeInitialLoc;
		NewEyeRelativeLoc.X = EyeInitialLoc.X + ShakeOffset; // 同步X轴颤动
		EyesFlipbookComponent->SetRelativeLocation(NewEyeRelativeLoc);
	}
	if (DroppedBoxes.Num())
	{
		for (AActor* Box : DroppedBoxes)
		{
			FCollisionShape CheckShape = FCollisionShape::MakeBox(FVector(29.0f, 29.0f, 29.0f)); 
			FCollisionQueryParams Params;
			FHitResult HitResult;
			Params.AddIgnoredActors(DroppedBoxes);
			FVector Start = Box->GetActorLocation();
			FVector End = Start + FVector(0,0,-5.0f); 
			if (GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, CheckShape, Params))
			{
				Cast<ABoxActor>(DroppedBoxes[0])->Box->SetLinearDamping(20.0f);
				UE_LOG(LogTemp, Log, TEXT("下落阻尼20"));
				break;
			}
				Cast<ABoxActor>(DroppedBoxes[0])->Box->SetLinearDamping(1.0f);
				UE_LOG(LogTemp, Log, TEXT("下落阻尼1"));
		}
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
	PlayerInputComponent->BindAction(TEXT("Remove Box"),IE_Pressed,this,&AABoxBot::RemoveDroppedBoxes);
}

void AABoxBot::RightFunction(float AxisValue)
{
	if (AxisValue==0)
	{
		FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,10));
		FVector CurrentVel = BoxBody->GetPhysicsLinearVelocity();
		BoxBody->SetPhysicsLinearVelocity(FVector(0, 0, CurrentVel.Z));
		bClockSpawnLeft=false;
		bClockSpawnRight=false;
		if (bIsInAir)return;
		FootFlipbookComponent->SetFlipbook(StandPaperFlipbook);
		return;
	}
	FVector CurrentVelocity = BoxBody->GetPhysicsLinearVelocity();
	if (bIsSpawnMode)
	{
		BoxBody->SetPhysicsLinearVelocity(FVector(0,0,CurrentVelocity.Z));
		if (AxisValue>0)
		{
			if (!bClockSpawnRight)
			{
				OnSpawnRight();
				bClockSpawnLeft=false;
				bClockSpawnRight=true;
			}
			return;
		}
		if (AxisValue<0)
		{
			if (!bClockSpawnLeft)
			{
				OnSpawnLeft();
				bClockSpawnLeft=true;
				bClockSpawnRight=false;
			}
			return;
		}
		
	}
	if (bIsPutDown)return;
	if (!bIsInAir||!CheckIsHooked())//检测是否在空中或者有方块接触地面
	{
		uint8 RotationYaw = (AxisValue > 0) ? 180 : 0;
		PlayerXVector=(AxisValue > 0) ? 1 : -1;
		EyesFlipbookComponent->SetRelativeRotation(FRotator(0,RotationYaw,0));
		FootFlipbookComponent->SetRelativeRotation(FRotator(0,RotationYaw,0));
		FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,6));
		
		//float DeltaTime = GetWorld()->GetDeltaSeconds();
		float Speed = 200;
		float TargetVelX = AxisValue * Speed;
		//检测撞墙清空速度，防止挂墙或者卡墙
		float UAxisValue=(AxisValue > 0) ? 1.0f : -1.0f;
		FCollisionShape CheckShape = FCollisionShape::MakeBox(FVector(29.0f, 29.0f, 29.0f)); 
		FCollisionQueryParams Params;
		FHitResult HitResult;
		Params.AddIgnoredActor(this);
		Params.AddIgnoredActors(BoxChain);
		bool bHitWall = false;
		FVector Start = BoxBody->GetComponentLocation();
		FVector End = Start + FVector((UAxisValue * 5.0f),0,0); 
		//本体有没有撞墙
		bool IsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, CheckShape, Params);
		if (IsHit)
		{
			AActor* HitActor = HitResult.GetActor();
			ABoxActor* HitBox = Cast<ABoxActor>(HitActor);
			
			if (!HitBox)//撞到的不是箱子
			{
				bHitWall = true;
			}
			else
			{
				if (DroppedBoxes.Num())
				{
					for (AActor* DroppedBox : DroppedBoxes)
					{
						if (!IsValid(DroppedBox)) continue;
						Start = DroppedBox->GetActorLocation();
						End = Start + FVector((UAxisValue * 5.0f),0,0); 
						FCollisionQueryParams DropParams;
						DropParams.AddIgnoredActor(this);
						DropParams.AddIgnoredActors(DroppedBoxes);
						FHitResult DropHitResult;
						if (GetWorld()->SweepSingleByChannel(DropHitResult, Start, End, FQuat::Identity, ECC_Visibility, CheckShape, DropParams))
						{
							bHitWall = true;
							break; 
						}
					}
				}
			}
			
		}
		else //盒子有没有撞墙
		{
			for (AActor* Box : BoxChain)
			{
				if (!IsValid(Box)) continue;
				Start = Box->GetActorLocation();
				End = Start + FVector((UAxisValue * 5.0f),0,0); 

				if (GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, CheckShape, Params))
				{
					bHitWall = true;
					break; 
				}
			}
		}
		if (bHitWall)
		{
			TargetVelX = 0.0f;
		}
		
		FVector DeltaLoc = FVector(TargetVelX /** DeltaTime*/ , 0, 0);
		BoxBody->SetPhysicsLinearVelocity(FVector( DeltaLoc.X,0,CurrentVelocity.Z));
		//AddActorLocalOffset(DeltaLoc);
		if (bIsInAir)return;
		FootFlipbookComponent->SetFlipbook(RunPaperFlipbook);
	}
	else //悬空且挂在悬崖上立即停止移动
	{
		BoxBody->SetPhysicsLinearVelocity(FVector(0.0f, 0.0f, CurrentVelocity.Z));
	}
	
}
void AABoxBot::JumpFunction()
{
	FVector CurrentVelocity = BoxBody->GetPhysicsLinearVelocity();
	if (bIsSpawnMode)
	{
		BoxBody->SetPhysicsLinearVelocity(FVector(0,0,CurrentVelocity.Z));
		return;
	}
	if (bIsPutDown)return;
	if (!bIsInAir)
	{
		BoxBody->SetPhysicsLinearVelocity(FVector(CurrentVelocity.X,CurrentVelocity.Y,400));
		//BoxBody->AddImpulse(FVector(0,0,25000));
		//BoxCollision->AddImpulse(GetActorUpVector().RotateAngleAxis(35.f,FVector::LeftVector)*25000.f);
	}
	
}

bool AABoxBot::CheckIsOnGround()
{
	FVector Start = BoxBody->GetComponentLocation();
	//UE_LOG(LogTemp, Log, TEXT("X: %f, Y: %f, Z: %f"), Start.X, Start.Y, Start.Z);
	FVector End = Start + (FVector::DownVector * 50.0f);
	FVector BoxExtent = FVector(30.0f, 30.0f, 1.0f);
	FCollisionShape SweepShape = FCollisionShape::MakeBox(BoxExtent);
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActor(this);
	CollisionParameters.AddIgnoredActors(BoxChain);
	FHitResult HitResult;
	bool IsOnGround = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, SweepShape, CollisionParameters);
	return IsOnGround;
}

void AABoxBot::SpawnBox(FVector Direction)
{
	if (!bIsSpawnMode ) return;
	RemoveDroppedBoxes();
	FVector SpawnLoc ;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
    if (BoxChain.Num()>0)//身上有方块
    {
    	SpawnLoc = BoxChain.Last()->GetActorLocation()+ (Direction * 64.0f);
    	FVector Start = BoxChain.Last()->GetActorLocation();
    	float TraceDist = (Direction.Z < 0) ? 59.0f : 35.0f; //判断检测方向，如果向下就检测一个方块的距离，否则只检测短距离
    	FVector End = Start + (Direction * TraceDist);
    	FHitResult HitResult;
    	FCollisionQueryParams CollisionParameters;
    	CollisionParameters.AddIgnoredActor(BoxChain.Last());
    	
    	FCollisionShape CheckShape = FCollisionShape::MakeBox(FVector(29.0f, 29.0f, 29.0f));

    	bool IsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, CheckShape, CollisionParameters);
    	//判断是回收还是生成新的方块
    	if (IsHit)//回收或向下生成
    	{
    		UE_LOG(LogTemp, Log, TEXT("111111"));
    		bool bIsRetracting = false;
    		if (BoxChain.Num()==1)
    		{
    			if (HitResult.GetComponent() == BoxBody)
    			{
				   
    				AActor* BoxToKill = BoxChain.Pop();
    				if (IsValid(BoxToKill))
    				{
    					BoxToKill->Destroy();
    				}
    				bIsRetracting=true;
    			}
    		}
    		if (BoxChain.Num()>=2)
    		{
    			if (HitResult.GetActor() == this)return;
    			if (HitResult.GetActor() == BoxChain[BoxChain.Num()-2])
    			{
    				AActor* BoxToKill = BoxChain.Pop();
    				if (IsValid(BoxToKill))
    				{
    					BoxToKill->Destroy();
    				}
    				UE_LOG(LogTemp, Log, TEXT("%d"),BoxChain.Num());
    				bIsRetracting=true;
    			}
    		}
    		if (bIsRetracting) return;
    		if (Direction.Z >= 0) //不是向下
    		{
    			return; 
    		}
    		if (Direction.Z < 0) //向下检测到非自身的物体
    		{
    			UPrimitiveComponent* HitComp = HitResult.GetComponent();
    			if (HitComp && HitComp->GetCollisionResponseToChannel(ECC_Pawn) == ECR_Block)//判断检测到的是什么（预留设计）
    			{
    				float CurrentGap = FMath::Abs(Start.Z - HitResult.ImpactPoint.Z-30);
    				UE_LOG(LogTemp, Log, TEXT("%f"),CurrentGap);
    				if (CurrentGap < 60.0f)//距离不够塞下一个方块
    				{
					    UE_LOG(LogTemp, Log, TEXT("CurrentGap < 60.0f"));
    					float LiftHeight = 62 - CurrentGap;//需要抬升的高度
    					bool bRoofBlocked = false; 
    					
    					FHitResult RoofHit;
    					FCollisionQueryParams RoofParams;
    					RoofParams.AddIgnoredActor(this);
    					RoofParams.AddIgnoredActors(BoxChain);
    					//检测玩家本身上方有没有空位
    					if (GetWorld()->SweepSingleByChannel(RoofHit, GetActorLocation(), GetActorLocation() + FVector(0,0,LiftHeight), FQuat::Identity, ECC_Visibility, CheckShape, RoofParams))
    					{
    						bRoofBlocked = true;
    					}
    					//检查每一个箱子上方是不是有空位
    					if (!bRoofBlocked)
    					{
    						for (AActor* Box : BoxChain)
    						{
    							if (!IsValid(Box)) continue;
    							FVector BoxStart = Box->GetActorLocation();
    							FVector BoxEnd = BoxStart + FVector(0,0,LiftHeight);

    							if (GetWorld()->SweepSingleByChannel(RoofHit,BoxStart, BoxEnd, FQuat::Identity, ECC_Visibility,CheckShape, RoofParams))
    							{
    								bRoofBlocked = true;
    								break; 
    							}
    						}
    					}
    					if (bRoofBlocked) return;//如果没有位置就不生成
    					if (BoxChain.Num()>=MaxBoxNumber)return;
    					BoxBody->SetPhysicsLinearVelocity(FVector::ZeroVector);
    					AddActorWorldOffset(FVector(0, 0, LiftHeight), false);//先抬高玩家
    					
    					SpawnLoc.Z = HitResult.ImpactPoint.Z + 28;
					    UE_LOG(LogTemp, Log, TEXT("%f"),LiftHeight);
    				}
    			}
    		}
    	}
    }
    else//身上没方块
    {
    	if (Direction.Z<0)return;
    	BoxYVector=Direction.X;
    	SpawnLoc = GetActorLocation() + (Direction * 64.0f);
    	FVector Start = BoxBody->GetComponentLocation();
    	FVector End = Start + (Direction * 35.0f);
    	FHitResult HitResult;
    	FCollisionQueryParams CollisionParameters;
    	CollisionParameters.AddIgnoredActor(this);
    	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility,CollisionParameters))return;
    	
    }
	if (BoxChain.Num()>=MaxBoxNumber)return;
	//生成
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
	
	if (bIsSpawnMode)
	{
		OnSpawnDown();
		return;
	}
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
	bIsPutDown=true;
	PutDownBox();
}

void AABoxBot::EndPutDownBox()
{
	if (bIsSpawnMode)return;
	AddActorWorldOffset(FVector(0, 0, 10), true);
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, -12.0f));
	bIsPutDown=false;
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

void AABoxBot::PutDownBox()
{
    if (BoxChain.Num() == 0) return;
    AActor* LeaderBox = BoxChain[0]; 
	Cast<ABoxActor>(LeaderBox)->SpriteComponent->SetSprite(BoxSheepB);
    if (!IsValid(LeaderBox)) return;

    for (int32 i = 1; i < BoxChain.Num(); i++)
    {
        AActor* ChildBox = BoxChain[i];
        if (IsValid(ChildBox))
        {
            FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, true);
            ChildBox->AttachToActor(LeaderBox, AttachRules);
        	
            ABoxActor* MyBox = Cast<ABoxActor>(ChildBox);
            if (MyBox && MyBox->Box)
            {
            	MyBox->BotPhysMat->Restitution = 0.0f;  
            	MyBox->BotPhysMat->Friction = 1.0f;     
            	MyBox->BotPhysMat->Density = 1.0f;
            	MyBox->BotPhysMat->FrictionCombineMode=EFrictionCombineMode::Max;
            	MyBox->BotPhysMat->RestitutionCombineMode=EFrictionCombineMode::Min;
            	
            	MyBox->Box->SetPhysMaterialOverride(MyBox->BotPhysMat);
            	MyBox->SpriteComponent->SetSprite(BoxSheepB);
            	MyBox->Box->SetCollisionProfileName(TEXT("BlockAll"));
            }
        }
    }
	
    FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
    LeaderBox->DetachFromActor(DetachRules);
	
    ABoxActor* MyLeader = Cast<ABoxActor>(LeaderBox);
    if (MyLeader && MyLeader->Box)
    {
    	MyLeader->Box->SetCollisionProfileName(TEXT("BlockAll"));
    	
    	MyLeader->BotPhysMat->Restitution = 0.0f;  
    	MyLeader->BotPhysMat->Friction = 1.0f;     
    	MyLeader->BotPhysMat->Density = 1.0f;
    	MyLeader->BotPhysMat->FrictionCombineMode=EFrictionCombineMode::Max;
    	MyLeader->BotPhysMat->RestitutionCombineMode=EFrictionCombineMode::Min;
        	
    	MyLeader->Box->SetPhysMaterialOverride(MyLeader->BotPhysMat);
    	
    	MyLeader->Box->SetMassOverrideInKg(NAME_None, 100.0f, true);
    	
    	MyLeader->Box->SetLinearDamping(1.0f);
    	MyLeader->Box->GetBodyInstance()->bLockXRotation = true;
    	MyLeader->Box->GetBodyInstance()->bLockYRotation = true;
    	MyLeader->Box->GetBodyInstance()->bLockZRotation = true;
    	
    	//MyLeader->AddActorWorldOffset(FVector(0, 0, 2.0f), false, nullptr, ETeleportType::TeleportPhysics);
        
    	MyLeader->Box->RecreatePhysicsState();//刷新物理
    	MyLeader->Box->SetSimulatePhysics(true);
    }
	DroppedBoxes = BoxChain;
	
    BoxChain.Empty();     
    bIsSpawnMode = false; 

    bClockSpawnLeft = false;
    bClockSpawnRight = false;
}

void AABoxBot::RemoveDroppedBoxes()
{
	if (DroppedBoxes.Num() == 0) return;
	for (AActor* Box : DroppedBoxes)
	{
		if (IsValid(Box))
		{
			Box->Destroy(); 
		}
	}
	DroppedBoxes.Empty();
}

void AABoxBot::ThrowBox(float ThrowVector)
{
	if (BoxChain.Num() == 0) return;
    AActor* LeaderBox = BoxChain[0]; 
	Cast<ABoxActor>(LeaderBox)->SpriteComponent->SetSprite(BoxSheepB);
    if (!IsValid(LeaderBox)) return;

    for (int32 i = 1; i < BoxChain.Num(); i++)
    {
        AActor* ChildBox = BoxChain[i];
        if (IsValid(ChildBox))
        {
            FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, true);
            ChildBox->AttachToActor(LeaderBox, AttachRules);
        	
            ABoxActor* MyBox = Cast<ABoxActor>(ChildBox);
            if (MyBox && MyBox->Box)
            {
            	MyBox->BotPhysMat->Restitution = 0.0f;  
            	MyBox->BotPhysMat->Friction = 1.0f;     
            	MyBox->BotPhysMat->Density = 1.0f;
            	MyBox->BotPhysMat->FrictionCombineMode=EFrictionCombineMode::Max;
            	MyBox->BotPhysMat->RestitutionCombineMode=EFrictionCombineMode::Min;
            	
            	MyBox->Box->SetPhysMaterialOverride(MyBox->BotPhysMat);
            	MyBox->SpriteComponent->SetSprite(BoxSheepB);
            	MyBox->Box->SetCollisionProfileName(TEXT("BlockAll"));
            }
        }
    }
	
    FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
    LeaderBox->DetachFromActor(DetachRules);
	
    ABoxActor* MyLeader = Cast<ABoxActor>(LeaderBox);
    if (MyLeader && MyLeader->Box)
    {
    	MyLeader->Box->SetCollisionProfileName(TEXT("BlockAll"));
    	
    	MyLeader->BotPhysMat->Restitution = 0.0f;  
    	MyLeader->BotPhysMat->Friction = 1.0f;     
    	MyLeader->BotPhysMat->Density = 1.0f;
    	MyLeader->BotPhysMat->FrictionCombineMode=EFrictionCombineMode::Max;
    	MyLeader->BotPhysMat->RestitutionCombineMode=EFrictionCombineMode::Min;
        	
    	MyLeader->Box->SetPhysMaterialOverride(MyLeader->BotPhysMat);
    	
    	MyLeader->Box->SetMassOverrideInKg(NAME_None, 100.0f, true);
    	
    	MyLeader->Box->SetLinearDamping(1.0f);
    	MyLeader->Box->GetBodyInstance()->bLockXRotation = true;
    	MyLeader->Box->GetBodyInstance()->bLockYRotation = true;
    	MyLeader->Box->GetBodyInstance()->bLockZRotation = true;
    	
    	//MyLeader->AddActorWorldOffset(FVector(0, 0, 2.0f), false, nullptr, ETeleportType::TeleportPhysics);
        
    	MyLeader->Box->RecreatePhysicsState();//刷新物理
    	MyLeader->Box->SetSimulatePhysics(true);
    }
	MyLeader->Box->AddImpulse(FVector(20000*ThrowVector,0,40000));
	DroppedBoxes = BoxChain;
	
    BoxChain.Empty();     
    bIsSpawnMode = false; 

    bClockSpawnLeft = false;
    bClockSpawnRight = false;
}


void AABoxBot::BeginSpawnBox()
{
	if (BoxChain.Num())//如果身上有方块直接扔出去
	{
		if (BoxYVector*PlayerXVector<0)//如果方块在背面，执行放下
		{
			PutDownBox();
			return;
		}
		ThrowBox(PlayerXVector);
		bIsThrowing=true;
		return;
	}
	bIsThrowing=false;
	bIsSpawnMode=true;
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, 0.0f));
	EyesFlipbookComponent->SetFlipbook(SpawnBoxEyesFlipbook);
}

void AABoxBot::EndSpawnBox()
{
	bIsSpawnMode=false;
	//结束放置时如果脚下有生成的方块，直接执行放下方块
	FCollisionShape CheckShape = FCollisionShape::MakeBox(FVector(29.0f, 29.0f, 29.0f)); 
	FCollisionQueryParams Params;
	FHitResult HitResult;
	Params.AddIgnoredActor(this);
	FVector Start = BoxBody->GetComponentLocation();
	FVector End = Start + FVector(0,0,-5); 
	GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, CheckShape, Params);
	if (BoxChain.Contains(HitResult.GetActor()))
		//多人时可能需要修改
		//
		//
		//
	{
		PutDownBox();
	}
	if (!bIsThrowing)
	{
		AddActorWorldOffset(FVector(0, 0, 10), true);
		BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, -12));
	}
	EyesFlipbookComponent->SetFlipbook(EyesFlipbook);
	if (BodySpriteComponent)
	{
		BodySpriteComponent->SetRelativeLocation(BodySpriteInitialRelativeLoc);
	}
	// 表情组件位置复位
	if (EyesFlipbookComponent)
	{
		EyesFlipbookComponent->SetRelativeLocation(FVector(0,5,0));
	}
}
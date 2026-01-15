// Fill out your copyright notice in the Description page of Project Settings.


#include "Players/ABoxBot.h"

#include "PaperFlipbookComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PaperFlipbook.h"
#include "Components/SphereComponent.h"
#include "Actors/BoxActor.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

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
	BotPhysMat = CreateDefaultSubobject<UPhysicalMaterial>(TEXT("BotPhysMat"));//物理材质
	BodySpriteComponent=CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("BodySpriteComponent"));
	EyesFlipbookComponent=CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("EyesFlipbookComponent"));
	Wheel1=CreateDefaultSubobject<USphereComponent>(TEXT("Wheel1"));
	Wheel2=CreateDefaultSubobject<USphereComponent>(TEXT("Wheel2"));
	
	//设置组件所在位置,身体和表情的序列素材组件在BoxBody身体碰撞盒子上,脚的序列动画组件在BoxFoot脚的球形碰撞体上
	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);
	BodySpriteComponent->SetupAttachment(BoxBody);
	BodySpriteInitialRelativeLoc = BodySpriteComponent->GetRelativeLocation();// 记录初始相对位置
	BoxFoot->SetupAttachment(BoxBody);
	FootFlipbookComponent->SetupAttachment(BoxFoot);
	EyesFlipbookComponent->SetupAttachment(BoxBody);
	Wheel1->SetupAttachment(BoxBody);
	Wheel2->SetupAttachment(BoxBody);
	
	//设置物理材质的初始化属性
	BotPhysMat->Restitution = 0.0f;  //弹性
	BotPhysMat->Friction = 0.0f;     //摩擦力
	BotPhysMat->Density = 1.0f;		 //密度
	BotPhysMat->FrictionCombineMode=EFrictionCombineMode::Min;
	BotPhysMat->RestitutionCombineMode=EFrictionCombineMode::Min;
	
	//配置身体的碰撞体盒子的基础属性
	BoxBody->SetPhysMaterialOverride(BotPhysMat);
	BoxBody->SetBoxExtent(FVector(29.5f, 29.5f, 29.5f)); //设置大小
	BoxBody->SetSimulatePhysics(true);//开启boxbody的物理模拟,true:可以被推动,具有物理性;false:只有碰撞检测的效果
	//锁定碰撞体盒子的旋转
	BoxBody->GetBodyInstance()->bLockXRotation = true;
	BoxBody->GetBodyInstance()->bLockYRotation = true;
	BoxBody->GetBodyInstance()->bLockZRotation = true;
	BoxBody->SetCollisionProfileName(TEXT("Pawn")); //碰撞配置预设为Pawn
	BoxBody->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);//视线检测通道改为阻挡,可以被Visibility检测到
	BoxBody->SetMassOverrideInKg(NAME_None, 100.0f, true);
	//配置脚的球形碰撞体的基础属性
	BoxFoot->SetPhysMaterialOverride(BotPhysMat);
	BoxFoot->SetSphereRadius(29.5f);
	BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, -12.0f));//相对Boxbody往下位移-12
	BoxFoot->SetCollisionProfileName(TEXT("Pawn")); 
	BoxFoot->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);//PhysicsOnly:仅参与物理模拟（被碰撞推动、受力），不响应射线 / 重叠检测
	BoxFoot->SetMassOverrideInKg(NAME_None, 0.0f, true);//BoxFoot的整体质量设为0
	
	Wheel1->SetPhysMaterialOverride(BotPhysMat);
	Wheel2->SetPhysMaterialOverride(BotPhysMat);
	Wheel1->SetMassOverrideInKg(NAME_None, 0.0f, true);
	Wheel2->SetMassOverrideInKg(NAME_None, 0.0f, true);
	Wheel1->SetRelativeLocation(FVector(-29.5f, 0.f, -29.5f));
	Wheel2->SetRelativeLocation(FVector(29.5f, 0.f, -29.5f));
	Wheel1->SetSphereRadius(2.5f);
	Wheel2->SetSphereRadius(2.5f);
	Wheel1->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Wheel2->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	SpringArm->SetRelativeRotation(FRotator(0,-90,0));
	SpringArm->TargetArmLength = 4500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 0.8f; 
	SpringArm->CameraLagMaxDistance = 400.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 200.0f);
	
	Camera->FieldOfView = 15.0f;
	PlayerXVector=-1.0f;
	BoxYVector=0;
	RemainingBoxNumber=MaxBoxNumber;
	
	BodySpriteComponent->SetSprite(BodySprite);
	
	EyesFlipbookComponent->SetFlipbook(EyesFlipbook);
	EyesFlipbookComponent->SetRelativeLocation(FVector(0,5,0));
	
	FootFlipbookComponent->SetFlipbook(StandPaperFlipbook);
	FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,10));
	
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
	RemainingBoxNumber=MaxBoxNumber;
	BodySpriteComponent->SetSprite(BodySprite);
	EyesFlipbookComponent->SetFlipbook(EyesFlipbook);
	EyesFlipbookComponent->SetRelativeLocation(FVector(0,5,0));
	FootFlipbookComponent->SetFlipbook(StandPaperFlipbook);
	FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,10));
	
}

void AABoxBot::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	APlayerController* PC = Cast<APlayerController>(NewController);
	if (PC)
	{
		MyID = UGameplayStatics::GetPlayerControllerID(PC);
		UploadtoGameInstance();
	}
	if (MyID == 0)
	{
		BoxBody->SetCollisionObjectType(COLLISION_P1);
		BoxFoot->SetCollisionObjectType(COLLISION_P1);
		Wheel1->SetCollisionObjectType(COLLISION_P1);
		Wheel2->SetCollisionObjectType(COLLISION_P1);
	}
	else if (MyID == 1)
	{
		BoxBody->SetCollisionObjectType(COLLISION_P2);
		BoxFoot->SetCollisionObjectType(COLLISION_P2);
		Wheel1->SetCollisionObjectType(COLLISION_P2);
		Wheel2->SetCollisionObjectType(COLLISION_P2);
	}
}

// Called every frame
void AABoxBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 检测地面状态
	bool bCurrentOnGround = CheckIsOnGround();
	bool bJustLanded = !bWasOnGround && bCurrentOnGround;
	bIsInAir = !bCurrentOnGround;
	bWasOnGround = bCurrentOnGround;
	
	// 落地瞬间：播放下蹲动画 + 启动计时器
	if (bJustLanded && FootFlipbookComponent && SquatPaperFlipbook)
	{
		// 切换下蹲动画
		bIsPlayingSquat = true;
		//将身体表情和腿同步往下相对位移-4
		FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,6));
		BodySpriteComponent->SetRelativeLocation(FVector(0,0,-4));
		EyesFlipbookComponent->SetRelativeLocation(FVector(0,5,-4));
		FootFlipbookComponent->SetFlipbook(SquatPaperFlipbook);
		UE_LOG(LogTemp, Log, TEXT("Tick：切换下蹲动画"));	

		// 取消之前的计时器
		GetWorld()->GetTimerManager().ClearTimer(SquatTimerHandle);

		// 2秒后切换为站立动画
		GetWorld()->GetTimerManager().SetTimer(
			SquatTimerHandle,
			this,
			&AABoxBot::SwitchToStandFlipbook,
			0.1f,
			false
		);
	}
	
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
		ABoxActor* Leader = Cast<ABoxActor>(DroppedBoxes[0]);
		if (Leader && Leader->Box)
		{
			float ZSpeed = Leader->Box->GetPhysicsLinearVelocity().Z;
			
			if (ZSpeed > 1.0f)
			{
				Leader->Box->SetLinearDamping(0.05f); 
			}
			else
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
						//UE_LOG(LogTemp, Log, TEXT("下落阻尼20"));
						break;
					}
					Cast<ABoxActor>(DroppedBoxes[0])->Box->SetLinearDamping(0.05f);
					//UE_LOG(LogTemp, Log, TEXT("下落阻尼1"));
				}
			}
		}
	}
	bool bWantsToStand = !bIsSpawnMode && !bIsPutDown && !bIsThrowing;
	bool bIsCrouching=BoxFoot->GetRelativeLocation().Z > -10.0f;
	if (bWantsToStand && bIsCrouching)
	{
		TryStandUp();
	}
}

// Called to bind functionality to input
void AABoxBot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AABoxBot::RightFunction(float AxisValue)
{
	if (AxisValue==0)
	{
		if (bIsPlayingSquat==false)//添加了一个是否在播放下蹲动画的判断,不在下蹲时下使用站姿状态
		{
			BodySpriteComponent->SetRelativeLocation(FVector(0,0,0));
			EyesFlipbookComponent->SetRelativeLocation(FVector(0,5,0));
			FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,10));
		}
		FVector CurrentVel = BoxBody->GetPhysicsLinearVelocity();
		BoxBody->SetPhysicsLinearVelocity(FVector(0, 0, CurrentVel.Z));
		bClockSpawnLeft=false;
		bClockSpawnRight=false;
		if (bIsInAir)return;
		if (bIsPlayingSquat==false)//添加了一个是否在播放下蹲动画的判断,不在下蹲时下使用站姿状态
		{
			FootFlipbookComponent->SetFlipbook(StandPaperFlipbook);
		}
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
		//跑步状态下,腿往下位移-4
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
			AABoxBot* HitPlayer = Cast<AABoxBot>(HitActor);
			
			if (!HitBox)//撞到的不是箱子
			{
				if (!HitPlayer) 
				{
					bHitWall = true;
				}
			}
			else
			{
				if (DroppedBoxes.Num())
				{
					Start=GetActorLocation();
					End=Start + FVector(0,0,-15); 
					FCollisionQueryParams AParams;
					AParams.AddIgnoredActor(this);
					FHitResult AHitResult;
					GetWorld()->SweepSingleByChannel(AHitResult, Start, End, FQuat::Identity, ECC_Visibility, CheckShape, AParams);
					if (AHitResult.GetActor())
					{UE_LOG(LogTemp, Log, TEXT("%s"),*AHitResult.GetActor()->GetName());}
					if (DroppedBoxes.Contains(AHitResult.GetActor()))
					{
						bHitWall = true;
						UE_LOG(LogTemp, Log, TEXT("L"));
					}
						
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
							if (!Cast<AABoxBot>(DropHitResult.GetActor())) 
							{
								
								
								bHitWall = true;
								break; 
							} 
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
					if (!Cast<AABoxBot>(HitResult.GetActor())) 
					{
						bHitWall = true;
						break; 
					}
					
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
		// 1. 先切换为下蹲动画
		if (FootFlipbookComponent && SquatPaperFlipbook)
		{
			//将身体表情和腿同步往下相对位移-4
			bIsPlayingSquat =true;
			FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,6));
			BodySpriteComponent->SetRelativeLocation(FVector(0,0,-4));
			EyesFlipbookComponent->SetRelativeLocation(FVector(0,5,-4));
			FootFlipbookComponent->SetFlipbook(SquatPaperFlipbook);			
			UE_LOG(LogTemp, Log, TEXT("JumpFunction：切换下蹲动画"));
		}

		// 2. 取消之前可能未执行的计时器（避免重复触发）
		GetWorld()->GetTimerManager().ClearTimer(SquatTimerHandle);

		// 3. 设置2秒后切换为跳跃动画
		GetWorld()->GetTimerManager().SetTimer
		(
			SquatTimerHandle,
			this,
			&AABoxBot::SwitchToJumpFlipbook,//秒后执行的函数
			0.1f, //下蹲时长
			false//不循环
		);
		
		//BoxBody->SetPhysicsLinearVelocity(FVector(CurrentVelocity.X,CurrentVelocity.Y,400));//这段代码移动至SwitchToJumpFlipbook函数中,在计时结束才会触发

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
    	float TraceDist = (Direction.Z < 0) ? 59.0f : 6.0f; //判断检测方向，如果向下就检测一个方块的距离，否则只检测短距离
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
    					RemainingBoxNumber++;
    					UploadtoGameInstance();
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
    					RemainingBoxNumber++;
    					UploadtoGameInstance();
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
	if (BoxChain.Num() >= MaxBoxNumber) return;

	//生成
	ABoxActor* NewBox = GetWorld()->SpawnActor<ABoxActor>(ABoxActor::StaticClass(), SpawnLoc, FRotator::ZeroRotator, SpawnParams);
	if (NewBox)
	{
		
		NewBox->SpriteComponent->SetSprite(BoxA);

	
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);
		NewBox->AttachToComponent(GetRootComponent(), AttachmentRules);
		
		ECollisionChannel MyChannel = (MyID == 0) ? COLLISION_P1 : COLLISION_P2;
		ECollisionChannel OtherChannel = (MyID == 0) ? COLLISION_P2 : COLLISION_P1;
    
		NewBox->Box->SetCollisionObjectType(MyChannel); 
		
		NewBox->Box->SetCollisionResponseToAllChannels(ECR_Block); 
		NewBox->Box->SetCollisionResponseToChannel(MyChannel, ECR_Ignore); 
		NewBox->Box->SetCollisionResponseToChannel(OtherChannel, ECR_Block); 
	
		NewBox->Wheel->SetCollisionObjectType(MyChannel);
		NewBox->Wheel->SetCollisionResponseToAllChannels(ECR_Block);
		NewBox->Wheel->SetCollisionResponseToChannel(MyChannel, ECR_Ignore);
		NewBox->Wheel->SetCollisionResponseToChannel(OtherChannel, ECR_Block);
		
	
		BoxChain.Add(NewBox);
		RemainingBoxNumber--;
		UploadtoGameInstance();
		
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
	/*TArray<AActor*> OutTeam;
	TSet<AActor*> Visited;
	CollectJumpTeam(this,OutTeam,Visited);
	if (CanTeamJump(OutTeam))
	{
		MoveUp(OutTeam);
		BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, -12.0f));
	}*/
	
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
	Cast<ABoxActor>(LeaderBox)->SpriteComponent->SetSprite(BoxB);
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
            	MyBox->SpriteComponent->SetSprite(BoxB);
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
	RemainingBoxNumber=MaxBoxNumber;
	UploadtoGameInstance();
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

//切换为跳跃动画
void AABoxBot::SwitchToJumpFlipbook()
{
	bIsPlayingSquat = false;
	UE_LOG(LogTemp, Log, TEXT("JSQ01计时器结束：切换为跳跃动画"));
	if (FootFlipbookComponent && JumpPaperFlipbook)
	{
		// 施加跳跃力
		/*FVector CurrentVelocity = BoxBody->GetPhysicsLinearVelocity();
		BoxBody->SetPhysicsLinearVelocity(FVector(CurrentVelocity.X,CurrentVelocity.Y,400));*/
		TArray<AActor*> OutTeam;
		TSet<AActor*> Visited;
		CollectJumpTeam(this,OutTeam,Visited);
		if (CanTeamJump(OutTeam))
		{
			ExecuteTeamJump(OutTeam);
		}
		//将脚部组件动画且为跳跃
		FootFlipbookComponent->SetRelativeLocation(FVector(0,-5,10));
		FootFlipbookComponent->SetFlipbook(JumpPaperFlipbook);
	}
}

// 切换为站立动画
void AABoxBot::SwitchToStandFlipbook()
{
	bIsPlayingSquat = false;
	UE_LOG(LogTemp, Log, TEXT("JSQ02计时器触发：切换为站立动画"));
	if (FootFlipbookComponent && StandPaperFlipbook)
	{
		BodySpriteComponent->SetRelativeLocation(FVector(0,0,0));
		EyesFlipbookComponent->SetRelativeLocation(FVector(0,5,0));
		FootFlipbookComponent->SetFlipbook(StandPaperFlipbook);
	}
}

void AABoxBot::CollectJumpTeam(AActor* CurrentActor, TArray<AActor*>& OutTeam, TSet<AActor*>& Visited)
{
	if (!CurrentActor||Visited.Contains(CurrentActor))return;//判断当前物体存不存在或是不是已经被扫描过
	
	AActor* RootActor=CurrentActor;
	while (RootActor->GetAttachParentActor())
	{
		RootActor=RootActor->GetAttachParentActor();//获取当前对象的根对象	
	}
	
	if (Visited.Contains(RootActor))return;//如果跟对象已经被记录，跳出
	TArray<AActor*> CurrentFamilyMembers;
	if (AABoxBot* Bot=Cast<AABoxBot>(RootActor))
	{
		Visited.Add(Bot);
		OutTeam.Add(Bot);
		CurrentFamilyMembers.Add(Bot);
		for (AActor* ChildActor : Bot->BoxChain)
		{
			Visited.Add(ChildActor);
			OutTeam.Add(ChildActor);
			CurrentFamilyMembers.Add(ChildActor);
		}
	}
	else
	{
		Visited.Add(RootActor);
		OutTeam.Add(RootActor);
		CurrentFamilyMembers.Add(RootActor);
		
		TArray<AActor*> AttachedActors;
		RootActor->GetAttachedActors(AttachedActors);
		for (AActor* Member : AttachedActors)
		{
			if (Member && !Visited.Contains(Member))
			{
				Visited.Add(Member);
				OutTeam.Add(Member);
				CurrentFamilyMembers.Add(Member);
			}
		}
	}
	for (AActor* Member : CurrentFamilyMembers)
	{
		bool bIsPlayer = Member->IsA<AABoxBot>();
		bool bIsBox = Member->IsA<ABoxActor>(); 
        
		if (bIsPlayer || bIsBox)
		{
			FVector Start = Member->GetActorLocation();
			FVector End = Start + FVector(0,0,20);
			TArray<FHitResult> HitResults;
			FCollisionQueryParams CollisionParameters;
			CollisionParameters.AddIgnoredActor(Member);
			CollisionParameters.AddIgnoredActor(this);
			FCollisionShape CheckShape = FCollisionShape::MakeBox(FVector(29.0f, 29.0f, 29.0f));

			bool IsHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Visibility, CheckShape, CollisionParameters);
			
			if (IsHit)
			{
				for (const FHitResult& Hit : HitResults)
				{
					AActor* TopActor = Hit.GetActor();
                    
					if (IsValid(TopActor) && (TopActor->IsA<AABoxBot>() || TopActor->IsA<ABoxActor>()))
					{
						CollectJumpTeam(TopActor, OutTeam, Visited);
					}
				}
			}
		}
	}
}

bool AABoxBot::CanTeamJump(const TArray<AActor*>& TeamMembers)
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionParameters;
	CollisionParameters.AddIgnoredActors(TeamMembers);
	CollisionParameters.AddIgnoredActor(this);
	for (AActor* Member : TeamMembers)
	{
		if (!IsValid(Member)) continue;
		FVector Start = Member->GetActorLocation();
		FVector End = Start + FVector(0,0,10);
		
		FCollisionShape CheckShape = FCollisionShape::MakeBox(FVector(29.0f, 29.0f, 29.0f));

		bool IsHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_Visibility, CheckShape, CollisionParameters);
			
		if (IsHit)
		{
			return false;
		}
	}
	return true;
}

void AABoxBot::ExecuteTeamJump(const TArray<AActor*>& TeamMembers)
{
	TArray<AActor*> SortedTeam = TeamMembers;
	SortedTeam.Sort([](const AActor& A, const AActor& B) {
		return A.GetActorLocation().Z < B.GetActorLocation().Z;
	});
	for (int32 i = 0; i < SortedTeam.Num(); i++)
	{
		AActor* Member = SortedTeam[i];
		UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Member->GetRootComponent());
		if (Prim&&Prim->IsSimulatingPhysics())
		{
			FVector CurrentVelocity = Prim->GetPhysicsLinearVelocity();
			float SpeedBonus = i * 10.0f;
			Prim->SetPhysicsLinearVelocity(FVector(CurrentVelocity.X,CurrentVelocity.Y,400/*+SpeedBonus*/));
			FVector a = Prim->GetPhysicsLinearVelocity();
			UE_LOG(LogTemp, Log, TEXT("%s,%f"), *Member->GetName(),a.Z);
			
		}
	}
}

void AABoxBot::MoveUp(const TArray<AActor*>& TeamMembers)
{
	for (AActor* Member:TeamMembers)
	{
		if (!IsValid(Member)) continue;
		
		if (Member->GetAttachParentActor() == nullptr)
		{
			UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Member->GetRootComponent());
			if (Prim)
			{
				
				Prim->AddLocalOffset(FVector(0, 0, 10), true);
				
			}
		}
	}
}

void AABoxBot::TryStandUp()
{
	TArray<AActor*> OutTeam;
	TSet<AActor*> Visited;
	CollectJumpTeam(this,OutTeam,Visited);
	if (CanTeamJump(OutTeam))
	{
		MoveUp(OutTeam);
		BoxFoot->SetRelativeLocation(FVector(0.f, 0.f, -12));
	}
}

void AABoxBot::UploadtoGameInstance()
{
	UMyGameInstance* GameInstance =Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));


	if (IsValid(GameInstance))
	{
		//将当前类的RemainingBoxNumber值赋给GameInstance中的G_P1RemainingBoxNumber
		if (MyID==0)
		{
			GameInstance->G_P1RemainingBoxNumber=this->RemainingBoxNumber;
		}
		if (MyID==1)
		{
			GameInstance->G_P2RemainingBoxNumber=this->RemainingBoxNumber;
		}
	}
	else UE_LOG(LogTemp, Log, TEXT("ABoxBot：GameInstance无效，无法同步剩余箱子数"));
}


void AABoxBot::ThrowBox(float ThrowVector)
{
	if (BoxChain.Num() == 0) return;
    AActor* LeaderBox = BoxChain[0]; 
	Cast<ABoxActor>(LeaderBox)->SpriteComponent->SetSprite(BoxB);
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
            	MyBox->SpriteComponent->SetSprite(BoxB);
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
	MyLeader->Box->AddImpulse(FVector(12000*ThrowVector,0,35000));
	DroppedBoxes = BoxChain;
	
    BoxChain.Empty();     
    bIsSpawnMode = false; 

    bClockSpawnLeft = false;
    bClockSpawnRight = false;
	RemainingBoxNumber=MaxBoxNumber;
	UploadtoGameInstance();
}


void AABoxBot::BeginSpawnBox()
{
	if (BoxChain.Num())//如果身上有方块直接扔出去
	{
		if (BoxYVector*PlayerXVector<0)//如果方块在背面，执行放下
		{
			PutDownBox();
			bIsThrowing=true;
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

	EyesFlipbookComponent->SetFlipbook(EyesFlipbook);
	
	
	//将身体复原到原位
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
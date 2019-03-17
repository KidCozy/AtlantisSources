// Fill out your copyright notice in the Description page of Project Settings.

#include "Dragon.h"
#include "DRController.h"
#include "DRAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "time.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "FireBall.h"

// Sets default values
ADragon::ADragon()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Head = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HEAD"));
	Tail = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TAIL"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Dragon(TEXT("/Game/DesertDragon/Meshes/SK_DesertDragon.SK_DesertDragon"));
	if (SK_Dragon.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Dragon.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_FIRE(TEXT("ParticleSystem'/Game/Action_FX/ParticleSystems/Fire_Bullet/Fire_Shot.Fire_Shot'"));
	if (P_FIRE.Succeeded())
	{
		Effect->SetTemplate(P_FIRE.Object);
		Effect->bAutoActivate = false;
	}

	Effect->SetupAttachment(RootComponent);

	GetCapsuleComponent()->SetCapsuleHalfHeight(224.0f);
	GetCapsuleComponent()->SetCapsuleRadius(148.0f);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -230.0f), FRotator(0.0f, -90.0f, 0.0f));

	AIControllerClass = ADRController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);

	Head->SetupAttachment(RootComponent);
	Head->SetCapsuleHalfHeight(119.0f);
	Head->SetCapsuleRadius(78.0f);
	Head->SetRelativeLocationAndRotation(FVector(340.0f, 0.0f, -20.0f), FRotator(0.0f, 90.0f, 0.0f));
	Head->SetCollisionProfileName(TEXT("Pawn"));

	Tail->SetupAttachment(RootComponent);
	Tail->SetCapsuleHalfHeight(280.0f);
	Tail->SetCapsuleRadius(31.0f);
	Tail->SetRelativeLocationAndRotation(FVector(-520.0f, 0.0f, -130.0f), FRotator(0.0f, 90.0f, 0.0f));
	Tail->SetCollisionProfileName(TEXT("Pawn"));

	IsAttacking = false;
	IsAlert = false;
}

// Called when the game starts or when spawned
void ADragon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADragon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADragon::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADragon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	DRAni = Cast<UDRAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != DRAni);
	DRAni->OnMontageEnded.AddDynamic(this, &ADragon::OnAttackMontageEnded);
	DRAni->OnAttackHitCheck.AddUObject(this, &ADragon::AttackCheck);
	DRAni->OnDash.AddUObject(this, &ADragon::Dash);
	DRAni->OnFireBall.AddUObject(this, &ADragon::FIreBall);

	Pattern = AttackPatten::GrAttack3;
}

void ADragon::Attack()
{
	if (IsAttacking) return;

	//Patten = AttackPatten::GrAttack1;
	
	switch (Pattern)
	{
	case AttackPatten::GrAttack1:
		DRAni->PlayAttacckMontage(Pattern);
		IsAttacking = true;
		break;
	case AttackPatten::GrAttack2:
		DRAni->PlayAttacckMontage(Pattern);
		IsAttacking = true;
		break;
	case AttackPatten::GrAttack3:
		DRAni->PlayAttacckMontage(Pattern);
		IsAttacking = true;
		break;
	}
	
}

void ADragon::AttackCheck()
{
	float AttackRange = 150.0f;
	float AttackRadius = 50.0f;
	switch (Pattern)
	{
	case AttackPatten::GrAttack1:

		break;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation() + GetActorForwardVector()*500.0f+ GetActorRightVector() * -75.0f +GetActorUpVector() * -60.0f, //FVector(-100.0f, 480.0f, -60.0f),
		GetActorLocation() + GetActorForwardVector()*500.0f + GetActorRightVector() * 75.0f + GetActorUpVector() * -60.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(50.0f),
		Params
	);


//#if ENABLE_DRAW_DEBUG
//
//	FVector TraceVec = GetActorForwardVector() * AttackRange;
//	FVector Center = GetActorLocation()+ GetActorForwardVector()*500.0f + GetActorUpVector() * -60.0f + TraceVec * 0.5f;
//	float HalfHeight = AttackRange * 0.5f + AttackRadius;
//	FQuat CapsuleRot = FRotationMatrix::MakeFromY(TraceVec).ToQuat();
//	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//	float DebugLifeTime = 5.0f;
//
//	DrawDebugCapsule(GetWorld(),
//		Center,
//		HalfHeight,
//		AttackRadius,
//		CapsuleRot,
//		DrawColor,
//		false,
//		DebugLifeTime);
//
//#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			//ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
		}
	}

}

void ADragon::Alert()
{
	if (IsAttacking) return;
	DRAni->PlayAttacckMontage(4);
	IsAlert = true;
}

bool ADragon::SelectPattern()
{
	srand((unsigned int)time(0));
	Pattern = (AttackPatten)(rand() % 3 + 1);

	switch (Pattern)
	{
	case Noting:
		break;
	case GrAttack1:
		return true;
	case GrAttack2:
		return true;
	case GrAttack3:
		return false;
		break;
	default:
		break;
	}
	return false;
}

void ADragon::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	IsAttacking = false;
	IsAlert = false;
	OnAlertEnd.Broadcast();
	OnAttackEnd.Broadcast();

}

void ADragon::Dash()
{
	
}

void ADragon::FIreBall()
{
	Effect->Activate(true);
	Effect->OnSystemFinished.AddDynamic(this, &ADragon::OnEffectFinished);
	auto Fire = GetWorld()->SpawnActor<AFireBall>(GetActorLocation() + GetActorForwardVector() * 500.0f, GetActorRotation());
}

void ADragon::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	Effect->Activate(false);
}



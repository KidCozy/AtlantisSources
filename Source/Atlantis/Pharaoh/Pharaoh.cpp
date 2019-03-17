// Fill out your copyright notice in the Description page of Project Settings.

#include "Pharaoh.h"
#include "PAAnimInstance.h"
#include "PHAIController.h"
#include "DrawDebugHelpers.h"
#include "time.h"
#include "Meteor.h"
#include "NavigationSystem.h"
#include "AttackRangeUI.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
APharaoh::APharaoh()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> 
		SK_Pharaoh(TEXT("SkeletalMesh'/Game/Frank_RPG_Dual/Skeletal_Meshes/Frank_Dual.Frank_Dual'"));
	if (SK_Pharaoh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Pharaoh.Object);
	}

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Monster"));

	RootComponent->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));
	
	GetMesh()->RelativeLocation = FVector(0, 0, -90);
	GetMesh()->RelativeRotation = FRotator(0, -90, 0);
	GetMesh()->SetCollisionProfileName(TEXT("MonsterMesh"));

	AIControllerClass = APHAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);


	IsAttacking = false;
	NowState = State::Nomal;

}

// Called when the game starts or when spawned
void APharaoh::BeginPlay()
{
	Super::BeginPlay();

	AttackRangeUI = (GetWorld()->SpawnActor<AAttackRangeUI>(FVector::ZeroVector, FRotator::ZeroRotator));
	AttackRangeUI->SetActive();
}

// Called every frame
void APharaoh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APharaoh::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APharaoh::PostInitializeComponents()
{
	Super::PostInitializeComponents();


	PHAni = Cast<UPAAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != PHAni);




	PHAni->OnMontageEnded.AddDynamic(this, &APharaoh::OnAttackMontageEnded);
	PHAni->OnAttackHitCheck.AddUObject(this, &APharaoh::AttackCheck);
	PHAni->OnSkill.AddUObject(this, &APharaoh::Skill);

	Pattern = AttackPatten::PhAttack1;

}

void APharaoh::Attack()
{
	if (IsAttacking) return;

	//Patten = AttackPatten::GrAttack1;

	switch (Pattern)
	{
	case AttackPatten::PhAttack1:
		PHAni->PlayAttacckMontage(Pattern);
		IsAttacking = true;
		break;
	case AttackPatten::PhAttack2:
		PHAni->PlayAttacckMontage(Pattern);
		IsAttacking = true;
		break;
	case AttackPatten::PhAttack3:
		PHAni->PlayAttacckMontage(Pattern);
		IsAttacking = true;
		break;
	case AttackPatten::PhAttack4:
		PHAni->PlayAttacckMontage(Pattern);
		IsAttacking = true;
		AttackRangeUI->SetActive();
		break;
	}
}

void APharaoh::AttackCheck()
{
	float AttackRange = 150.0f;
	float AttackRadius = 50.0f;
	switch (Pattern)
	{
	case AttackPatten::PhAttack4:
		AttackRangeUI->SetActive();
		break;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		//GetActorLocation() + GetActorForwardVector()*500.0f + GetActorRightVector() * -75.0f + GetActorUpVector() * -60.0f, //FVector(-100.0f, 480.0f, -60.0f),
		//GetActorLocation() + GetActorForwardVector()*500.0f + GetActorRightVector() * 75.0f + GetActorUpVector() * -60.0f,
		GetActorLocation() + GetActorUpVector() * -160.0f, //FVector(-100.0f, 480.0f, -60.0f),
		GetActorLocation() + GetActorForwardVector()*484.0f + GetActorUpVector() * -160.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_Pawn,
		FCollisionShape::MakeSphere(80.0f),
		Params
	);


	#if ENABLE_DRAW_DEBUG
	
		FVector TraceVec = GetActorForwardVector() * 484.0f;
		//FVector Center = GetActorLocation()+ GetActorForwardVector()*500.0f + GetActorUpVector() * -60.0f + TraceVec * 0.5f;
		FVector Center = GetActorLocation() + GetActorUpVector() * -160.0f + TraceVec * 0.5f;
		//float HalfHeight = AttackRange * 0.5f + AttackRadius;
		float HalfHeight = 484.0f * 0.5f + 80.0f;
		FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
		FColor DrawColor = bResult ? FColor::Green : FColor::Red;
		float DebugLifeTime = 5.0f;
	
		DrawDebugCapsule(GetWorld(),
			Center,
			HalfHeight,
			AttackRadius,
			CapsuleRot,
			DrawColor,
			false,
			DebugLifeTime);
	
	#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			//AttackArea->SetActive(true);
			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
		}
	}

}


void APharaoh::Skill()
{
	FVector CharacterPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector SpawnPos = FVector(CharacterPos.X, CharacterPos.Y, 3000.0f);
	auto Meteor = GetWorld()->SpawnActor<AMeteor>(SpawnPos, FRotator::ZeroRotator);
}

int APharaoh::SelectPattern(float Speed, FVector ChPos)
{
	srand((unsigned int)time(0));
	Pattern = (AttackPatten)(rand() % 3 + 1);

	if(Speed > 1000.0f)
	Pattern = AttackPatten::PhAttack4;

	switch (Pattern)
	{
	case Noting:
		break;
	case PhAttack1:
		return 1;
	case PhAttack2:
		return 2;
	case PhAttack3:
		return 3;
	case PhAttack4:
		AttackRangeUI->SetActorLocation(ChPos);
		GetCharacterMovement()->MaxWalkSpeed = Speed;
		return 4;
	default:
		break;
	}
	return 0;
}



void APharaoh::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}





// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterWeapon.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "ParticleDefinitions.h"
// Sets default values
ACharacterWeapon::ACharacterWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	TrailPt = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TRAIL"));

	RootComponent = Weapon;


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/Externals/PinkGirl/Blade/SK_Edge.SK_Edge"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PT_TRAIL(TEXT("/Game/Externals/M5VFXRPG1/Particles/VF_Trail/VF_pt_02.VF_pt_02"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}

	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	

}

// Called when the game starts or when spawned
void ACharacterWeapon::BeginPlay()
{
	Super::BeginPlay();
	
 //	UGameplayStatics::SpawnEmitterAttached(TrailPt., Weapon, NAME_None, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, false,EPSCPoolMethod::None);
//	TrailPt->BeginTrails("root", "root", ETrailWidthMode::ETrailWidthMode_FromCentre, 1.0);
	
//	TrailPt->AttachToComponent(Weapon, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}
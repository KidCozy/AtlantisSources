// Fill out your copyright notice in the Description page of Project Settings.

#include "FireBall.h"

// Sets default values
AFireBall::AFireBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Col = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HITEFFECT"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> E_FIRE(TEXT("ParticleSystem'/Game/Action_FX/ParticleSystems/Fire_Bullet/Fire_Bullet.Fire_Bullet'"));
	if (E_FIRE.Succeeded())
	{
		Effect->SetTemplate(E_FIRE.Object);
		//Effect->bAutoActivate = false;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> E_HIT(TEXT("ParticleSystem'/Game/Action_FX/ParticleSystems/Fire_Bullet/Fire_Hit.Fire_Hit'"));
	if (E_HIT.Succeeded())
	{
		HitEffect->SetTemplate(E_HIT.Object);
		HitEffect->bAutoActivate = false;
	}

	//Col->set

	SetRootComponent(Col);
	Effect->SetupAttachment(RootComponent);
	HitEffect->SetupAttachment(RootComponent);

	Col->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
}

// Called when the game starts or when spawned
void AFireBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rot;
	rot = GetActorRotation();
	//AddActorLocalOffset
	AddActorWorldOffset(GetActorForwardVector()*50.0f);
}


// Fill out your copyright notice in the Description page of Project Settings.

#include "Meteor.h"
#include "AttackRangeUI.h"

// Sets default values
AMeteor::AMeteor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Col = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HITEFFECT"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> E_FIRE(TEXT("ParticleSystem'/Game/Action_FX/ParticleSystems/MagicBall/P_Meteor.P_Meteor'"));
	if (E_FIRE.Succeeded())
	{
		Effect->SetTemplate(E_FIRE.Object);
		//Effect->bAutoActivate = false;
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> E_HIT(TEXT("ParticleSystem'/Game/Action_FX/ParticleSystems/MagicBall/Hit.Hit'"));
	if (E_HIT.Succeeded())
	{
		HitEffect->SetTemplate(E_HIT.Object);
		HitEffect->bAutoActivate = false;
	}

	


	//Col->set

	Col->SetCollisionProfileName("Meteor");

	SetRootComponent(Col);
	Effect->SetupAttachment(RootComponent);
	HitEffect->SetupAttachment(RootComponent);

	Col->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));
	IsMove = true;

	HitEffect->OnSystemFinished.AddDynamic(this, &AMeteor::OnEffectFinished);
}

// Called when the game starts or when spawned
void AMeteor::BeginPlay()
{
	Super::BeginPlay();
	
	FVector SpawnPos(GetActorLocation().X, GetActorLocation().Y, 138.0f);

	UI = (GetWorld()->SpawnActor<AAttackRangeUI>(SpawnPos, FRotator::ZeroRotator));

}

void AMeteor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Col->OnComponentBeginOverlap.AddDynamic(this, &AMeteor::OnCollisionOverlap);
}

// Called every frame
void AMeteor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rot;
	rot = GetActorRotation();
	//AddActorLocalOffset
	if(IsMove)
	AddActorWorldOffset(GetActorUpVector()*-50.0f);
}

void AMeteor::OnCollisionOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OherCcomp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	IsMove = false;
	Effect->SetActive(false);
	HitEffect->SetActive(true);



	auto Inst_ = Cast<UPlayerGameInstance>(GetGameInstance());
	Inst_->HitShake(Inst_->CShakeBossMeteor, 5.0f);

	//Cast<UPlayerGameInstance>(&UGameInstance::UGameInstance())->CShakeBossMeteor;
	Col->SetWorldRotation(FRotator(0.0f, 0.0f, 0.0f));
	UI->End();

	ABCHECK(IsValid(Cast<APlayerCharacter>(OtherActor)));
	auto Character = Cast<APlayerCharacter>(OtherActor);
	Character->GetHit(this);
//	ABLOG_S(Warning);
}

void AMeteor::OnEffectFinished(UParticleSystemComponent * PSystem)
{
//	ABLOG_S(Warning);
	Destroy(this);
}




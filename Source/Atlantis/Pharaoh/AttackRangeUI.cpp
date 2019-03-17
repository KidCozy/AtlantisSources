// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackRangeUI.h"

// Sets default values
AAttackRangeUI::AAttackRangeUI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PLANE"));

	RootComponent = Plane;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> AT_PLANE(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (AT_PLANE.Succeeded())
	{
		Plane->SetStaticMesh(AT_PLANE.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterial> AT_MAT(TEXT("Material'/Game/Temp/Indicator_Mat.Indicator_Mat'"));
	if (AT_MAT.Succeeded())
	{
		Plane->SetMaterial(0,AT_MAT.Object);
	}

	RootComponent->SetWorldScale3D(FVector(3.0f, 3.0f, 1.0f));

	IsActive = true;

	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void AAttackRangeUI::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAttackRangeUI::End()
{
	Destroy(this);
}

void AAttackRangeUI::SetActive()
{
	if (IsActive) {
		SetActorHiddenInGame(IsActive);
		IsActive = false;
	}
	else
	{
		SetActorHiddenInGame(IsActive);
		IsActive = true;
	}

}


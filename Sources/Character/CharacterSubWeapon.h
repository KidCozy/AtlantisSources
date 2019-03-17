// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "GameFramework/Actor.h"
#include "CharacterSubWeapon.generated.h"

UCLASS()
class ATLANTIS_API ACharacterSubWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSubWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	UParticleSystemComponent* TrailPt;
	
};

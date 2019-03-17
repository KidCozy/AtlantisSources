// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "GameFramework/Actor.h"
#include "FireBall.generated.h"

UCLASS()
class ATLANTIS_API AFireBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Col)
	USphereComponent * Col;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent * Effect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent * HitEffect;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

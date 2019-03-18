// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "GameFramework/Actor.h"
#include "Meteor.generated.h"

UCLASS()
class ATLANTIS_API AMeteor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeteor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = Col)
	USphereComponent * Col;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent * Effect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent * HitEffect;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnCollisionOverlap(UPrimitiveComponent * OverlappedComp, AActor* OtherActor,UPrimitiveComponent* OherCcomp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult & SweepResult);

	bool IsMove;

	UPROPERTY()
	class AAttackRangeUI * UI;

private:

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);
};

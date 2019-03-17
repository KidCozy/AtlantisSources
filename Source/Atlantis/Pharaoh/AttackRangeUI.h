// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "GameFramework/Actor.h"
#include "AttackRangeUI.generated.h"

UCLASS()
class ATLANTIS_API AAttackRangeUI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackRangeUI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent * Plane;

	UPROPERTY()
	bool  IsActive;

	void End();

	void SetActive();
};

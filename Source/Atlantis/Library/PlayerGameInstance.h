// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "Engine/GameInstance.h"
#include "PlayerGameInstance.generated.h"


UCLASS()
class ATLANTIS_API UPlayerGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> CShakeBossMeteor;
	
	UPlayerGameInstance(const FObjectInitializer& ObjectInitializer);
	void Init();
	void HitShake(TSubclassOf<UCameraShake> CShake,float Scalar);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "AIController.h"
#include "PHAIController.generated.h"

/**
 * 
 */
UCLASS()
class ATLANTIS_API APHAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	APHAIController();
	virtual void Possess(APawn * InPawn)override;

	static const FName TargetKey;
	static const FName JumpAttackPosKey;
	static const FName DistanceToCharacterKey;
	static const FName NeedMoveKey;
	static const FName NeedTurnKey;
	static const FName NeedJumpKey;

private:

	UPROPERTY()
	class UBehaviorTree * BT;
	UPROPERTY()
	class UBlackboardData * BB;
};

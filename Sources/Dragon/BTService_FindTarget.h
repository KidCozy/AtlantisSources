// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindTarget.generated.h"

/**
 * 
 */
UCLASS()
class ATLANTIS_API UBTService_FindTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_FindTarget();

	virtual void TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)override;
};

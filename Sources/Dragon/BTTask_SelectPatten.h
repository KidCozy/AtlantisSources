// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SelectPatten.generated.h"

/**
 * 
 */
UCLASS()
class ATLANTIS_API UBTTask_SelectPatten : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTask_SelectPatten();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
};

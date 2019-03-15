// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SelectPatten.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DRController.h"
#include "Dragon.h"

UBTTask_SelectPatten::UBTTask_SelectPatten()
{
	NodeName = TEXT("SelectPattern");
}

EBTNodeResult::Type UBTTask_SelectPatten::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	auto Dragon = Cast<ADragon>(OwnerComp.GetAIOwner()->GetPawn());
	//if (Dragon == nullptr)
		//return EBTNodeResult::Failed;

	bool NeedMove = Dragon->SelectPattern();
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ADRController::NeedMoveKey, NeedMove);

	return EBTNodeResult::Succeeded;
}

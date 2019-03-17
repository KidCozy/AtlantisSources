// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Attack.h"
#include "PHAIController.h"
#include "Pharaoh.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Pharaoh = Cast<APharaoh>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Pharaoh)
		return EBTNodeResult::Failed;

	Pharaoh->Attack();
	IsAttacking = true;
	Pharaoh->OnAttackEnd.AddLambda([this]()->void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

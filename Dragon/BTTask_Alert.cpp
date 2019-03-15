// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Alert.h"
#include "DRController.h"
#include "Dragon.h"

UBTTask_Alert::UBTTask_Alert()
{

}
EBTNodeResult::Type UBTTask_Alert::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Dragon = Cast<ADragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Dragon)
		return EBTNodeResult::Failed;

	Dragon->Alert();
	IsAlert = true;
	Dragon->OnAlertEnd.AddLambda([this]()->void {
		IsAlert = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Alert::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAlert) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	


}

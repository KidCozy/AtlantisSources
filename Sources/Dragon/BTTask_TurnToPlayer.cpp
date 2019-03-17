// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToPlayer.h"
#include "DRController.h"
#include "Dragon.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToPlayer::UBTTask_TurnToPlayer()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Dragon = Cast<ADragon>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Dragon)
		return EBTNodeResult::Failed;

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADRController::TargetKey));
	if(nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Dragon->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Dragon->SetActorRotation(FMath::RInterpTo(Dragon->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 5.0f));

	return EBTNodeResult::Succeeded;
}

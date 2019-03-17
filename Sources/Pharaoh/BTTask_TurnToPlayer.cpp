// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_TurnToPlayer.h"
#include "DRController.h"
#include "Pharaoh.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToPlayer::UBTTask_TurnToPlayer()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToPlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Pharaoh = Cast<APharaoh>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Pharaoh)
		return EBTNodeResult::Failed;

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ADRController::TargetKey));
	if(nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - Pharaoh->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Pharaoh->SetActorRotation(FMath::RInterpTo(Pharaoh->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 5.0f));

	return EBTNodeResult::Succeeded;
}

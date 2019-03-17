// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_SelectPatten.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PHAIController.h"
#include "PlayerCharacter.h"
#include "Pharaoh.h"

UBTTask_SelectPatten::UBTTask_SelectPatten()
{
	NodeName = TEXT("SelectPattern");
}

EBTNodeResult::Type UBTTask_SelectPatten::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	auto Pharaoh = Cast<APharaoh>(OwnerComp.GetAIOwner()->GetPawn());
	if(Pharaoh == nullptr)
		return EBTNodeResult::Failed;

	//if (Dragon == nullptr)
		//return EBTNodeResult::Failed;

	auto Player = Cast<APlayerCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APHAIController::TargetKey));
	if (Player == nullptr)
		return EBTNodeResult::Failed;


	FVector AIPos = FVector(Pharaoh->GetActorLocation().X, Pharaoh->GetActorLocation().Y,0.0f);

	FVector ChPos = FVector(Player->GetActorLocation().X, Player->GetActorLocation().Y, 0.0f);

	float distance = FVector::Distance(AIPos, ChPos);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(APHAIController::DistanceToCharacterKey, distance);

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(APHAIController::JumpAttackPosKey, Player->GetActorLocation());

	int Pattern;

	FVector UiPos(Player->GetActorLocation().X, Player->GetActorLocation().Y, 138.0f);

	if (distance > 800.0f && distance < 1500.0f)
	{
		float speed = (distance * 600.0f) / 700.0f;
		Pattern = Pharaoh->SelectPattern(speed, UiPos);
		ABLOG(Warning, TEXT("Speed : %f"), speed);
	}else if (distance > 1500.0f && distance < 2000.0f)
	{
		float speed = (distance * 600.0f) / 500.0f;
		Pattern = Pharaoh->SelectPattern(speed, UiPos);
		ABLOG(Warning, TEXT("Speed : %f"), speed);
	}
	else if (distance > 2000.0f)
	{
		float speed = (distance * 600.0f) / 100.0f;
		Pattern = Pharaoh->SelectPattern(speed, UiPos);
		ABLOG(Warning, TEXT("Speed : %f"), speed);
	}
	else
	{
		Pattern = Pharaoh->SelectPattern(600.0f, UiPos);
	}

	switch (Pattern)
	{
	case 1:
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(APHAIController::NeedMoveKey, true);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(APHAIController::NeedTurnKey, true);
		break;
	case 2:
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(APHAIController::NeedMoveKey, true);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(APHAIController::NeedTurnKey, false);
		break;
	case 3:
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(APHAIController::NeedMoveKey, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(APHAIController::NeedJumpKey, false);
		break;
	case 4:
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(APHAIController::NeedMoveKey, false);
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(APHAIController::NeedJumpKey, true);
		break;
	default:
		break;
	}

	//bool NeedMove = Pharaoh->SelectPattern();
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(ADRController::NeedMoveKey, NeedMove);

	return EBTNodeResult::Succeeded;
}

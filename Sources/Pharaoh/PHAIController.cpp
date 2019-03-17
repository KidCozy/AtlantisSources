// Fill out your copyright notice in the Description page of Project Settings.

#include "PHAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName APHAIController::TargetKey(TEXT("Target"));
const FName APHAIController::JumpAttackPosKey(TEXT("JumpAttackPos"));
const FName APHAIController::DistanceToCharacterKey(TEXT("DistanceToCharacter"));
const FName APHAIController::NeedMoveKey(TEXT("NeedMove"));
const FName APHAIController::NeedTurnKey(TEXT("NeedTurn"));
const FName APHAIController::NeedJumpKey(TEXT("NeedJump"));

APHAIController::APHAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObj(TEXT("/Game/AI/BB_Pharaoh.BB_Pharaoh"));
	if (BBObj.Succeeded())
	{
		BB = BBObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObj(TEXT("/Game/AI/BT_Pharaoh.BT_Pharaoh"));
	if (BTObj.Succeeded())
	{
		BT = BTObj.Object;
	}
}

void APHAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);
	if (UseBlackboard(BB, Blackboard))
	{
		if (!RunBehaviorTree(BT))
		{
			//ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}


}
// Fill out your copyright notice in the Description page of Project Settings.

#include "DRController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName ADRController::TargetKey(TEXT("Target"));
const FName ADRController::NeedMoveKey(TEXT("NeedMove"));
const FName ADRController::GoPosKey(TEXT("GoPos"));

ADRController::ADRController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObj(TEXT("/Game/AI/BB_Dragon.BB_Dragon"));
	if (BBObj.Succeeded())
	{
		BB = BBObj.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObj(TEXT("/Game/AI/BT_Dragon.BT_Dragon"));
	if (BTObj.Succeeded())
	{
		BT = BTObj.Object;
	}
}

void ADRController::Possess(APawn * InPawn)
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

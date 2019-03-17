// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_FindTarget.h"
#include "PHAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_FindTarget::UBTService_FindTarget()
{
	NodeName = TEXT("FindTarget");
	Interval = 1.0f;
}

void UBTService_FindTarget::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UWorld * Wolrd = OwnerComp.GetAIOwner()->GetPawn()->GetWorld();

	ACharacter *Player = Cast<ACharacter>(Wolrd->GetFirstPlayerController()->GetPawn());
	if (Player == nullptr)
		return;

	//FVector PVec;
	//FVector DrVec;
	//FVector vec = FVector::ZeroVector;

	//PVec = Player->GetActorLocation();
	//DrVec = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

	//PVec.Normalize();
	//DrVec.Normalize();

	//vec.X = PVec.X+ 100.0f * FMath::Cos(FMath::FindDeltaAngleRadians());
	//vec.Y = PVec.Y + 100.0f * FMath::Sin(FMath::DegreesToRadians(90));

	//vec = FVector::CrossProduct(Player->GetActorLocation(), OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation());
	//vec.Normalize();
	//vec = vec * 10000;

	//float angle = FMath::Acos(FVector::DotProduct(PVec, DrVec));

	//PVec = Player->GetActorLocation();

	//vec.X = PVec.X + 100.0f * FMath::Cos(FMath::DegreesToRadians(angle));
	//vec.Y = PVec.Y + 100.0f * FMath::Sin(FMath::DegreesToRadians(angle));

	//vec.Z = 0;
	//vec.X = (Player->GetActorLocation().X + OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation().X) / 2;
	//vec.Y = 

	if (Player != NULL) {
	//	ABLOG(Warning, TEXT("%s"), *Player->GetName());
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(APHAIController::TargetKey, Player);
		
	}
}

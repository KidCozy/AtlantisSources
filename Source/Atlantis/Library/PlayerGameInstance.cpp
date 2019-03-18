// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerGameInstance.h"

UPlayerGameInstance::UPlayerGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UCameraShake> CSHAKE_BY_BOSS_METEOR(TEXT("/Game/Animations/AnimBP/Camera/Cshake_BY_Boss_Meteor.Cshake_BY_Boss_Meteor_C"));
	//	static ConstructorHelpers::FClassFinder<UCameraShake> CSHAKE_BY_BOSS_SMASH(TEXT("dsd"));



	if (CSHAKE_BY_BOSS_METEOR.Succeeded())
		CShakeBossMeteor = CSHAKE_BY_BOSS_METEOR.Class;
	//	static ConstructorHelpers::FClassFinder<UCameraShake> CSHAKE_BY_BOSS_DIVE(TEXT("DASD"));
	//	static ConstructorHelpers::FClassFinder<UCameraShake> CSHAKE_BY_PLAYER_ATTACK(TEXT("DASDS"));

}


void UPlayerGameInstance::Init() {
	
	
	


}

void UPlayerGameInstance::HitShake(TSubclassOf<UCameraShake> CShake, float Scalar) {
	FTimerHandle time_;
	if (CShake != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CShake, Scalar);
	}



	auto Character = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	Character->AudioComp->Play();

	Character->Camera->PostProcessSettings.SceneFringeIntensity = 5.0f;
	Character->GetWorldTimerManager().SetTimer(time_, Character, &APlayerCharacter::HitPostProcess, 0.5f, false);


}
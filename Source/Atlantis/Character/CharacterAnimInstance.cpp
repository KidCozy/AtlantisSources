// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimInstance.h"

UCharacterAnimInstance::UCharacterAnimInstance() {
	PawnForwardSpeed = 0.0f;
	PawnRightSpeed = 0.0f;	
	IsInAir = false;
	IsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Animations/AnimBP/PlayerCharacter_ATTACK_Montage.PlayerCharacter_ATTACK_Montage"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> WATTACK_MONTAGE(TEXT("AnimMontage'/Game/Animations/AnimBP/PlayerCharacterWATTACK.PlayerCharacterWATTACK'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SATTACK_MONTAGE(TEXT("/Game/Animations/AnimBP/PlayerCharacter_ATTACK_Montage.PlayerCharacter_ATTACK_Montage"));


	static ConstructorHelpers::FObjectFinder<UAnimMontage> SUBATTACK_MONTAGE(TEXT("/Game/Animations/AnimBP/PlayerCharacter_ATTACK2_Montage.PlayerCharacter_ATTACK2_Montage"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AIRATTACK_MONTAGE(TEXT("/Game/Animations/AnimBP/PlayerCharacter_AIRATTACK_Montage.PlayerCharacter_AIRATTACK_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DASH_MONTAGE(TEXT("/Game/Animations/AnimBP/PlayerCharacter_Dash_Montage.PlayerCharacter_Dash_Montage"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EVADE_MONTAGE(TEXT("AnimMontage'/Game/Animations/AnimBP/Evade/Frank_RPG_Dual_Evade_F_Montage.Frank_RPG_Dual_Evade_F_Montage'"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SKILL01_MONTAGE(TEXT("AnimMontage'/Game/Animations/AnimBP/PlayerCharacterGRABSMASH.PlayerCharacterGRABSMASH'"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> GETHIT_MONTAGE(TEXT("AnimMontage'/Game/Animations/AnimBP/Hit/PlayerCharacter_GETHIT.PlayerCharacter_GETHIT'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GETUP_MONTAGE(TEXT("AnimMontage'/Game/Animations/AnimBP/Hit/PlayerCharacter_GETUP.PlayerCharacter_GETUP'"));


	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

	if (WATTACK_MONTAGE.Succeeded()) {
		WAttackMontage = WATTACK_MONTAGE.Object;
	}

	if (SATTACK_MONTAGE.Succeeded()) {
		SAttackMontage = SATTACK_MONTAGE.Object;
	}

	if (SUBATTACK_MONTAGE.Succeeded())
	{
		SubAttackMontage = SUBATTACK_MONTAGE.Object;
	}

	if (AIRATTACK_MONTAGE.Succeeded()) {
		AirAttackMontage = AIRATTACK_MONTAGE.Object;
	}

	if (EVADE_MONTAGE.Succeeded()) {
		EvadeMontage = EVADE_MONTAGE.Object;
	}

	if (SKILL01_MONTAGE.Succeeded()) {
		Skill01Montage = SKILL01_MONTAGE.Object;
	}

	if (GETHIT_MONTAGE.Succeeded()) {
		GetHitMontage = GETHIT_MONTAGE.Object;
	}

	if (GETUP_MONTAGE.Succeeded()) {
		GetUpMontage = GETUP_MONTAGE.Object;
	}
}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto Pawn = TryGetPawnOwner();
	
	if (::IsValid(Pawn))
	{
		auto Character = Cast<APlayerCharacter>(Pawn);

		isEvade = Pawn->GetMovementComponent()->IsFalling();
	//	CurrentMachine = GetStateMachineInstance(0);
		isAttack = Character->isAttack;

		PawnForwardSpeed = Character->CurrentYSpeed;
		PawnRightSpeed = Character->CurrentXSpeed;

		eqpindex = Character->EquipIndex;

		CurrentCombo = Character->CurrentCombo;

		isDowned = Character->isDowned;
	//	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, FString::SanitizeFloat(PawnForwardSpeed) + FString::SanitizeFloat(PawnRightSpeed));
	}
}



void UCharacterAnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
	if (!isEvade) {

		if (ForwardDelta >= 0.5f) {
			Montage_Play(WAttackMontage, 1.0f);
		}
		
		else if (ForwardDelta <= -0.5f) {
			Montage_Play(SAttackMontage, 1.0f);
		}
		else if (RightDelta >= 0.5f) {

		}
		else {
			switch (eqpindex) {
			case 1:

				Montage_Play(AttackMontage, 1.0f);
				break;

			case 2:
				Montage_Play(SubAttackMontage, 1.0f);
				break;
			}
		}

		

	}
	else {
		Montage_Play(AirAttackMontage, 1.0f);
	}

	//GEngine->AddOnScreenDebugMessage(8, 3.0f, FColor::Green, IsInAir ? "TRUE" : "FALSE");
	
}
void UCharacterAnimInstance::PlayEvadeMontage() {
		ABCHECK(!Montage_IsPlaying(EvadeMontage)&&!isAttack && CurrentCombo <= 0);
			Montage_Play(EvadeMontage);
	
}
void UCharacterAnimInstance::PlaySkillMontage()
{
	ABCHECK(!IsDead);
	if (!isEvade) {

		Montage_Play(Skill01Montage, 1.0f);



	}

}

void UCharacterAnimInstance::PlayGetHitMontage() {

	Montage_Play(GetHitMontage, 1.0f);
}

void UCharacterAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	if (!isEvade)
	{
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), WAttackMontage);


		switch (eqpindex) {
		case 1:
			ABCHECK(Montage_IsPlaying(AttackMontage));
			Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
			break;

		case 2:
			ABCHECK(Montage_IsPlaying(SubAttackMontage));
			Montage_JumpToSection(GetAttackMontageSectionName(NewSection), SubAttackMontage);
			break;
		}
	
	}
	else
	{
		ABCHECK(Montage_IsPlaying(AirAttackMontage));
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AirAttackMontage);
	}
}

void UCharacterAnimInstance::JumpToSkillMontageSection(int32 NewSection)
{

	GEngine->AddOnScreenDebugMessage(7, 1.0f, FColor::Blue, FString::FormatAsNumber(NewSection));
	ABCHECK(!IsDead);
	if (!isEvade)
	{
		ABCHECK(Montage_IsPlaying(Skill01Montage));
		Montage_JumpToSection(GetSkillMontageSectionName(NewSection), Skill01Montage);


	}
}



void UCharacterAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();

}

void UCharacterAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UCharacterAnimInstance::AnimNotify_ActionShake()
{
	OnActionShake.Broadcast();
}

FName UCharacterAnimInstance::GetAttackMontageSectionName(int32 Section)
{

	if (!isEvade)
	{

		ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
		return FName(*FString::Printf(TEXT("Frank_RPG_Dual_Combo01_%d_IP"), Section));
		
	}
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Frank_RPG_Dual_Combo01_%d_IP"), Section));
}


FName UCharacterAnimInstance::GetSkillMontageSectionName(int32 Section)
{

	if (!isEvade)
	{

		ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);
		return FName(*FString::Printf(TEXT("Frank_RPG_Dual_Combo01_%d_IP"), Section));

	}
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);
	return FName(*FString::Printf(TEXT("Frank_RPG_Dual_Combo01_%d_IP"), Section));
}

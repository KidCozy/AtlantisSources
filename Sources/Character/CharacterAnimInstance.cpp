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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> FDASH_MONTAGE(TEXT("/Game/Animations/AnimBP/DashMontage/PlayerCharacter_FDash_Montage.PlayerCharacter_FDash_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BDASH_MONTAGE(TEXT("/Game/Animations/AnimBP/DashMontage/PlayerCharacter_BDash_Montage.PlayerCharacter_BDash_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RDASH_MONTAGE(TEXT("/Game/Animations/AnimBP/DashMontage/PlayerCharacter_RDash_Montage.PlayerCharacter_RDash_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LDASH_MONTAGE(TEXT("/Game/Animations/AnimBP/DashMontage/PlayerCharacter_LDash_Montage.PlayerCharacter_LDash_Montage"));
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> FEVADE_MONTAGE(TEXT("AnimMontage'/Game/Animations/AnimBP/Evade/Frank_RPG_Dual_Evade_F_Montage.Frank_RPG_Dual_Evade_F_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BEVADE_MONTAGE(TEXT("AnimMontage'/Game/Animations/AnimBP/Evade/Frank_RPG_Dual_Evade_B_Montage.Frank_RPG_Dual_Evade_B_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LEVADE_MONTAGE(TEXT("AnimMontage'/Game/Animations/AnimBP/Evade/Frank_RPG_Dual_Evade_L_Montage.Frank_RPG_Dual_Evade_L_Montage'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> REVADE_MONTAGE(TEXT("AnimMontage'/Game/Animations/AnimBP/Evade/Frank_RPG_Dual_Evade_R_Montage.Frank_RPG_Dual_Evade_R_Montage'"));


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
	if (FDASH_MONTAGE.Succeeded() && BDASH_MONTAGE.Succeeded() && RDASH_MONTAGE.Succeeded() && LDASH_MONTAGE.Succeeded()) {
		FDashMontage = FDASH_MONTAGE.Object;
		BDashMontage = BDASH_MONTAGE.Object;
		RDashMontage = RDASH_MONTAGE.Object;
		LDashMontage = LDASH_MONTAGE.Object;
	}

	//if (FEVADE_MONTAGE.Succeeded() && BEVADE_MONTAGE.Succeeded() && LEVADE_MONTAGE.Succeeded() && REVADE_MONTAGE.Succeeded()) {
	//	//EvadeMontage[0] = FEVADE_MONTAGE.Object;
	//	//EvadeMontage[1] = BEVADE_MONTAGE.Object;
	//	//EvadeMontage[2] = LEVADE_MONTAGE.Object;
	//	//EvadeMontage[3] = REVADE_MONTAGE.Object;
	//}


}

void UCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	auto Pawn = TryGetPawnOwner();
	
	if (::IsValid(Pawn))
	{
		isJump = Pawn->GetMovementComponent()->IsFalling();
	//	CurrentMachine = GetStateMachineInstance(0);
		isAttack = Cast<APlayerCharacter>(Pawn)->isAttack;

		PawnForwardSpeed = Cast<APlayerCharacter>(Pawn)->CurrentYSpeed;
		PawnRightSpeed = Cast<APlayerCharacter>(Pawn)->CurrentXSpeed;

		eqpindex = Cast<APlayerCharacter>(Pawn)->EquipIndex;

	//	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, FString::SanitizeFloat(PawnForwardSpeed) + FString::SanitizeFloat(PawnRightSpeed));
	}
}


void UCharacterAnimInstance::PlayFDashSequence() {
	ABCHECK(!IsDead);
//	ABCHECK(!Montage_IsPlaying(AttackMontage));
	Montage_Play(FDashMontage,1.0f);
}


void UCharacterAnimInstance::PlayBDashSequence() {
	ABCHECK(!IsDead);
//	ABCHECK(!Montage_IsPlaying(AttackMontage));
	Montage_Play(BDashMontage, 1.0f);
}

void UCharacterAnimInstance::PlayRDashSequence() {
	ABCHECK(!IsDead);
//ABCHECK(!Montage_IsPlaying(AttackMontage));
	Montage_Play(RDashMontage, 1.0f);
}

void UCharacterAnimInstance::PlayLDashSequence() {
	ABCHECK(!IsDead);
//	ABCHECK(!Montage_IsPlaying(AttackMontage));
	Montage_Play(LDashMontage, 1.0f);
}


void UCharacterAnimInstance::PlayAttackMontage()
{
	ABCHECK(!IsDead);
	if (!isJump) {
	
		ABCHECK(!Montage_IsPlaying(FDashMontage) || !Montage_IsPlaying(BDashMontage) || !Montage_IsPlaying(RDashMontage) || !Montage_IsPlaying(LDashMontage));

		if (true) {
			Montage_Play(WAttackMontage, 1.0f);
		}
		else if (PawnForwardSpeed <= 0.0f) {
			Montage_Play(SAttackMontage, 1.0f);
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

void UCharacterAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(!IsDead);
	
	if (!isJump)
	{

		if (true) {
			ABCHECK(Montage_IsPlaying(WAttackMontage));
			Montage_JumpToSection(GetAttackMontageSectionName(NewSection), WAttackMontage);
		}


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

void UCharacterAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();

}

void UCharacterAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UCharacterAnimInstance::GetAttackMontageSectionName(int32 Section)
{

	if (!isJump)
	{
		


		switch (eqpindex) {
		case 1:
			ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
			return FName(*FString::Printf(TEXT("Frank_RPG_Dual_Combo01_%d_IP"), Section));
			break;

		case 2:
			ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);
			return FName(*FString::Printf(TEXT("Frank_RPG_Dual_Combo02_%d_IP"), Section));

			break;
		}
		
	}
	else
	{

		ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);
		return FName(*FString::Printf(TEXT("Frank_RPG_Dual_Air_Combo01_%d_IP"), Section));
	}
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Frank_RPG_Dual_Combo01_%d_IP"), Section));
}

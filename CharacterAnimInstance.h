// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"

#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
/**
 * 
 */
UCLASS()
class ATLANTIS_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UCharacterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void GetCurrentAnimLength();
	
	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		float PawnForwardSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		float PawnRightSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		bool isJump;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		bool isAttack;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		bool isMoveable = true;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		int AttackStack = 0;


private:	
	void PlayAttack_01();


public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;

	void SetDeadAnim() { IsDead = true; }
	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);
	
	void PlayFDashSequence();
	void PlayBDashSequence();
	void PlayRDashSequence();
	void PlayLDashSequence();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* FDashMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* BDashMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* RDashMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* LDashMontage;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	//	UAnimMontage* EvadeMontage[4];
	
private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		int eqpindex;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SubAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* AirAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true)) // Combo 07
		UAnimMontage* WAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true)) // Combo 03
		UAnimMontage* SAttackMontage;


//	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"

#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnActionShakeDelegate);
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
	
	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		float PawnForwardSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		float PawnRightSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		float ForwardDelta;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		float RightDelta;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		bool isEvade;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		bool isAttack;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		bool isMoveable = true;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		int CurrentCombo = 0;

	UPROPERTY(EditAnywhere, BlueprintReadonly,
		Category = Pawn)
		bool isDowned;

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnActionShakeDelegate OnActionShake;
	

	void SetDeadAnim() { IsDead = true; }
	void PlayAttackMontage();
	void PlayEvadeMontage();
	void PlaySkillMontage();
	void PlayGetHitMontage();
	void JumpToAttackMontageSection(int32 NewSection);
	void JumpToSkillMontageSection(int32 NewSection);
	
	
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	//	UAnimMontage* EvadeMontage[4];

private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck();

	UFUNCTION()
		void AnimNotify_NextAttackCheck();

	UFUNCTION()
		void AnimNotify_ActionShake();

	FName GetAttackMontageSectionName(int32 Section);
	FName GetSkillMontageSectionName(int32 Section);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true)) // Combo 07
		UAnimMontage* WAttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true)) // Combo 03
		UAnimMontage* SAttackMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true)) // Combo 03
		UAnimMontage* EvadeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true)) // Combo 03
		UAnimMontage* Skill01Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true)) // Combo 03
		UAnimMontage* GetHitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true)) // Combo 03
		UAnimMontage* GetUpMontage;


//	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
		

};

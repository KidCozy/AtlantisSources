// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "Animation/AnimInstance.h"
#include "PAAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSkillDelegate);

/**
 * 
 */
UCLASS()
class ATLANTIS_API UPAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPAAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	void PlayAttacckMontage(int32 NewSection);
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnSkillDelegate OnSkill;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pharaoh, Meta = (AllowPrivateAccess = true))
	float CurrentPharaohSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage * AttackMontage;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_Skill();


	FName GetAttackMontageSectionName(int32 Section);
};

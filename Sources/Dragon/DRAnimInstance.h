// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "Animation/AnimInstance.h"
#include "DRAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDashDelegate);
DECLARE_MULTICAST_DELEGATE(FOnFireBallDelegate);

/**
 * 
 */
UCLASS()
class ATLANTIS_API UDRAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UDRAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

	void PlayAttacckMontage(int32 NewSection);

	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnDashDelegate OnDash;
	FOnFireBallDelegate OnFireBall;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dragon, Meta = (AllowPrivateAccess = true))
	float CurrentDragonSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Dragon, Meta = (AllowPrivateAccess = true))
	bool IsAlert;

	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category = Attack,Meta = (AllowPrivateAccess = true))
	UAnimMontage * AttackMontage;

	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_Dash();

	UFUNCTION()
	void AnimNotify_FireBall();

	FName GetAttackMontageSectionName(int32 Section);
};
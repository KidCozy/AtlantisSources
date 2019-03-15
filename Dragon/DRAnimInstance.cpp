// Fill out your copyright notice in the Description page of Project Settings.

#include "DRAnimInstance.h"
#include "Dragon.h"

UDRAnimInstance::UDRAnimInstance()
{
	CurrentDragonSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/AI/SK_DesertDragon_Skeleton_Montage.SK_DesertDragon_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UDRAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Dragon = Cast<ADragon>(TryGetPawnOwner());
	if (::IsValid(Dragon))
	{
		CurrentDragonSpeed = Dragon->GetVelocity().Size();
		//IsAlert = Dragon->GetAlert();
	}
}

void UDRAnimInstance::PlayAttacckMontage(int32 NewSection)
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection),AttackMontage);
	}
}

void UDRAnimInstance::AnimNotify_AttackHitCheck()
{
	//ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}

void UDRAnimInstance::AnimNotify_Dash()
{
	OnDash.Broadcast();
}

void UDRAnimInstance::AnimNotify_FireBall()
{
	OnFireBall.Broadcast();
}

FName UDRAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

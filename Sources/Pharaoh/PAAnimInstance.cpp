// Fill out your copyright notice in the Description page of Project Settings.

#include "PAAnimInstance.h"
#include "Pharaoh.h"

UPAAnimInstance::UPAAnimInstance()
{
	CurrentPharaohSpeed = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("AnimMontage'/Game/AI/Pharaoh.Pharaoh'"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UPAAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pharaoh = Cast<APharaoh>(TryGetPawnOwner());
	if (::IsValid(Pharaoh))
	{
		CurrentPharaohSpeed = Pharaoh->GetVelocity().Size();
		//IsAlert = Dragon->GetAlert();
	}
}

void UPAAnimInstance::PlayAttacckMontage(int32 NewSection)
{
	if (!Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage, 1.0f);
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
	}
}

void UPAAnimInstance::AnimNotify_AttackHitCheck()
{
	//ABLOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}

void UPAAnimInstance::AnimNotify_Skill()
{
	OnSkill.Broadcast();
}


FName UPAAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

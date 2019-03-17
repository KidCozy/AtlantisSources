

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "GameFramework/Character.h"
#include "Pharaoh.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

enum AttackPatten
{
	Noting,
	PhAttack1,
	PhAttack2,
	PhAttack3,
	PhAttack4
};

enum State
{
	Nomal,
	Groggy
};

UCLASS()
class ATLANTIS_API APharaoh : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APharaoh();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;

	float GetHp() { return Hp; }
	void SetHp(float NewHp) { Hp = NewHp; }

	void Attack();
	void AttackCheck();
	FOnAttackEndDelegate OnAttackEnd;

	void Skill();

	int SelectPattern(float Speed,FVector ChPos);


	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted);


private:
	UPROPERTY(EditInStanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float Hp;

	UPROPERTY(EditInStanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float JumpAttackSpeed;

	UPROPERTY()
	class UPAAnimInstance * PHAni;

	UPROPERTY()
	class AAttackRangeUI * AttackRangeUI;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;


	AttackPatten Pattern;

	State NowState;

	
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Atlantis.h"
#include "GameFramework/Character.h"
#include "Dragon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);
DECLARE_MULTICAST_DELEGATE(FonAlertEndDelegate);


enum AttackPatten
{
	Noting,
	GrAttack1,
	GrAttack2,
	GrAttack3
};


UCLASS()
class ATLANTIS_API ADragon : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ADragon();

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

	bool GetAlert() { return IsAlert; }
	void SetAlert(bool NewAlert) { IsAlert = NewAlert; }

	void Attack();
	void AttackCheck();
	FOnAttackEndDelegate OnAttackEnd;
	FonAlertEndDelegate OnAlertEnd;

	void Alert();

	bool SelectPattern();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted);

private:
	UPROPERTY(EditInStanceOnly,Category = Stat,Meta = (AllowPrivateAccess = true))
	float Hp;

	UPROPERTY()
	bool IsAlert; //경계중인지 아닌지

	UPROPERTY(VisibleAnywhere,Category = body)
	UCapsuleComponent * Head;

	UPROPERTY(VisibleAnywhere, Category = body)
	UCapsuleComponent * Tail;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent * Effect;
	
	UPROPERTY()
	class UDRAnimInstance * DRAni;

	UPROPERTY(VisibleInstanceOnly,BlueprintReadOnly,Category = Attack,Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	AttackPatten Pattern;

	FTimerHandle TimerHandle;

	void Dash();

	void FIreBall();

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);

};

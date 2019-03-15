// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Atlantis.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"
#include "PlayerCharacter.generated.h"

namespace direction {
	enum DIRECTION {
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT,
	};
}

using namespace direction;

UCLASS()
class ATLANTIS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
public:

	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void ViewTurnRight(float NewAxisValue);
	void ViewTurnUp(float NewAxisValue);
	
	void CheckFDash();
	void CheckBDash();
	void CheckRDash();
	void CheckLDash();
	

	void FDash();
	void BDash();
	void RDash();
	void LDash();

	void BeginAttack();
	void StopAttack();
	void Run();


	UPROPERTY(VisibleAnywhere)
	UCharacterMovementComponent* Movement;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShake> CameraShake;

	UPROPERTY(VisibleAnywhere)
	int AttackStack;

	UPROPERTY(VisibleAnywhere)
		int EquipIndex;

	UPROPERTY(VisibleAnywhere)
		float CurrentYSpeed;
	UPROPERTY(VisibleAnywhere)
		float CurrentXSpeed;

	UPROPERTY(EditAnywhere)
	bool isAttack;

	UPROPERTY(EditAnywhere)
	bool isMoveable;

	UPROPERTY(EditAnywhere)
		bool isRun;
	UPROPERTY(EditAnywhere)
		bool isWalk;
	UPROPERTY(EditAnywhere)
		bool isDashing;
	UPROPERTY(EditAnywhere)
		bool isDashable;

	bool CanSetWeapon();
	void SetWeapon(class ACharacterWeapon* NewWeapon);
	void SetWeapon(class ACharacterWeapon* NewWeapon, class ACharacterWeapon* NewWeapon2);
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ACharacterWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ACharacterWeapon* SubWeapon;
	
	UPROPERTY(BlueprintReadOnly)
	USoundCue* AttackSFX;

	UPROPERTY(BlueprintReadOnly)
	UAudioComponent* AudioComp;

FTimerHandle AttackDelay_;
	
	void Attack();

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnDashMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();
	void HitShake();
	void HitPostProcess();
	void DashResetTimer();

	void SwapWeapon_01();
	void SwapWeapon_02();
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;

	UPROPERTY()
		class UCharacterAnimInstance* ABAnim;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	FTimerHandle DashTimer_;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		FTimerHandle DashResetTimer_;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int FDashStack;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int BDashStack;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int RDashStack;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int LDashStack;


	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UForceFeedbackEffect* ForceFeedBack;

	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	//DIRECTION Direct;

};

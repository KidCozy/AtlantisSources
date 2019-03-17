// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "Atlantis.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
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


	void Evade();

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

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float CurrentYSpeed;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
		float CurrentXSpeed;

	UPROPERTY(EditAnywhere)
	bool isAttack;

	UPROPERTY(EditAnywhere)
	bool isMoveable;

	UPROPERTY(EditAnywhere)
		bool isRun;
	UPROPERTY(EditAnywhere)
		bool isWalk;

	bool CanSetWeapon();
	void SetWeapon(class ACharacterWeapon* NewWeapon);
	void SetWeapon(class ACharacterWeapon* NewWeapon, class ACharacterWeapon* NewWeapon2);
	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ACharacterWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ACharacterWeapon* SubWeapon;

	UPROPERTY(VisibleAnywhere)
		TArray<FHitResult> SK2_SingleContainer1;
	UPROPERTY(VisibleAnywhere)
		TArray<FHitResult> SK2_SingleContainer2;
	UPROPERTY(VisibleAnywhere)
		TArray<FHitResult> SK2_SingleContainer3;

	UPROPERTY(BlueprintReadOnly)
	USoundCue* AttackSFX;

	UPROPERTY(BlueprintReadOnly)
	UAudioComponent* AudioComp;

	FTimerHandle AttackDelay_;
	
	void Attack();
	void Skill01();
	void Skill02();


	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();
	void HitShake();
	void HitPostProcess();

	void SwapWeapon_01();
	void SwapWeapon_02();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsSkilling;

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
		UForceFeedbackEffect* ForceFeedBack;

	

};

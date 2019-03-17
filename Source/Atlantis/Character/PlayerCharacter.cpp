// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include"CharacterAnimInstance.h"
#include"CharacterWeapon.h"
#include"CharacterSubWeapon.h"
// Sets default values
APlayerCharacter::APlayerCharacter()
{
	
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//Movement = CreateDefaultSubobject<UMovementComponent>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AUDIO"));
//	CameraShake = CreateDefaultSubobject<UCameraShake>(TEXT("CAMERASHAKE"));

	
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 700;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = true;
	SpringArm->bUsePawnControlRotation = false;

	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;

	SpringArm->CameraLagSpeed = 7.5f;

	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(0, 0, 50));
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = 0.0f;

	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = true;
//	Camera->PostProcessSettings.GrainIntensity = 5.0f;

	Camera->PostProcessSettings.ChromaticAberrationStartOffset = 0.0f;
	//Camera->PostProcessSettings.ChromaticAberrationStartOffset = 10.0f;
//	FPostProcessSettings::BloomDirtMaskIntensity;
		// 5.0 ~ 0

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

	//GetCharacterMovement()->jumpheight = 800.0f;
	
	JumpMaxCount = 2;

	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_BODY(TEXT("/Game/Externals/PinkGirl/Character/Meshes/SK_Kurumi.SK_Kurumi"));

	if (SK_BODY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BODY.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		CHARACTER_ANIM(TEXT("/Game/Animations/AnimBP/PlayerCharacter_AnimBP.PlayerCharacter_AnimBP_C"));

	if (CHARACTER_ANIM.Succeeded()) {
		GetMesh()->SetAnimInstanceClass(CHARACTER_ANIM.Class);
	}

	static ConstructorHelpers::FClassFinder<UCameraShake>
		CAMERASHAKE(TEXT("/Game/Animations/AnimBP/Camera/Attack_CameraShake.Attack_CameraShake_C"));
	static ConstructorHelpers::FObjectFinder<UForceFeedbackEffect>
		FORCEFEEDBACK_CAMERASHAKE(TEXT("/Game/Animations/AnimBP/Camera/AttackFeedBack.AttackFeedBack"));


	if (CAMERASHAKE.Succeeded()) {
		CameraShake = CAMERASHAKE.Class;
	}

	if (FORCEFEEDBACK_CAMERASHAKE.Succeeded()) {
		ForceFeedBack = FORCEFEEDBACK_CAMERASHAKE.Object;
	}

	GetMesh()->RelativeLocation = FVector(0, 0, -90);
	GetMesh()->RelativeRotation = FRotator(0, -90, 0);

	CurrentCombo = 0;
	IsAttacking = false;
	MaxCombo = 4;
	AttackEndComboState();
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("PlayerCharacter"));
	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	Camera->PostProcessSettings.bOverride_SceneFringeIntensity = true;
	Camera->PostProcessSettings.bOverride_MobileHQGaussian = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldMethod = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldScale = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldMaxBokehSize = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldFarBlurSize = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldFarTransitionRegion = true;


//	Camera->PostProcessSettings.SceneFringeIntensity = 5.0f;
//	Camera->PostProcessSettings.DepthOfFieldMethod = EDepthOfFieldMethod::DOFM_BokehDOF;
//	Camera->PostProcessSettings.DepthOfFieldFarBlurSize = 5000.0f;
//	Camera->PostProcessSettings.DepthOfFieldMaxBokehSize = 5.0f;
//	Camera->PostProcessSettings.DepthOfFieldFocalDistance = 400.0f;
//	Camera->PostProcessSettings.DepthOfFieldFarTransitionRegion = 100000.0f;

	static ConstructorHelpers::FObjectFinder<USoundCue>ATTACK_SFX(TEXT("SoundCue'/Game/Sounds/Sword_Slash_SFX_C.Sword_Slash_SFX_C'"));

	if (ATTACK_SFX.Succeeded()) {
		AttackSFX = ATTACK_SFX.Object;
	}

	AudioComp->bAutoActivate = false;
	AudioComp->AttachTo(RootComponent);

	AudioComp->SetSound(AttackSFX);

	EquipIndex = 1;



}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	FName WeaponSocket(TEXT("hand_r"));

	//(Pitch=-14.627683,Yaw=-135.755875,Roll=97.125572)
	//(X=-10.935351,Y=3.138973,Z=3.343325)
		auto CurWeapon = GetWorld()->SpawnActor<ACharacterWeapon>(FVector(-10.935351f, 3.138973f, 3.343325f), FRotator(-14.627683f, -135.755875f, 97.12557f));
		if (nullptr != CurWeapon) {
			CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
			CurWeapon->SetActorRelativeLocation((FVector(-10.935351f, 3.138973f, 3.343325f)));
			CurWeapon->SetActorRelativeRotation((FRotator(-14.627683f, -135.755875f, 97.12557f)));
		}
}
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	ABAnim = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != ABAnim);

	

	ABAnim->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnAttackMontageEnded);
	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {
	
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
			ABAnim->JumpToSkillMontageSection(CurrentCombo);
		}
	});

	ABAnim->OnAttackHitCheck.AddUObject(this, &APlayerCharacter::AttackCheck);
}
// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
//	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::White, isWalk ? "WALKING" : "NOT WALKING");
	//GEngine->AddOnScreenDebugMessage(6, 3.0f, FColor::Emerald, IsAttacking ? "IsAttack : TRUE" : "IsAttack : FALSE");
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::ViewTurnRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::ViewTurnUp);

	PlayerInputComponent->BindAction("Evade", IE_Pressed, this, &APlayerCharacter::Evade);
	//PlayerInputComponent->BindAction("Evade", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
//	PlayerInputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacter::StopAttack);

	PlayerInputComponent->BindAction("SwapWeapon_01", IE_Pressed, this, &APlayerCharacter::SwapWeapon_01);
	PlayerInputComponent->BindAction("SwapWeapon_02", IE_Pressed, this, &APlayerCharacter::SwapWeapon_02);

	PlayerInputComponent->BindAction("Skill_01", IE_Pressed, this, &APlayerCharacter::Skill01);
	PlayerInputComponent->BindAction("Skill_02", IE_Pressed, this, &APlayerCharacter::Skill02);
}

void APlayerCharacter::SwapWeapon_01() {

	EquipIndex = 1;

	FName WeaponSocket(TEXT("hand_r"));
	FName SubWeaponSocket(TEXT("hand_l"));

	GetMesh()->RemoveSocketOverrides("hand_l");

	auto CurWeapon = GetWorld()->SpawnActor<ACharacterWeapon>(FVector(-10.935351f, 3.138973f, 3.343325f), FRotator(-14.627683f, -135.755875f, 97.12557f));
	if (nullptr != CurWeapon) {
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		CurWeapon->SetActorRelativeLocation((FVector(-10.935351f, 3.138973f, 3.343325f)));
		CurWeapon->SetActorRelativeRotation((FRotator(-14.627683f, -135.755875f, 97.12557f)));
	}
}

void APlayerCharacter::SwapWeapon_02() {

	EquipIndex = 2;

	FName WeaponSocket(TEXT("hand_r"));
	FName SubWeaponSocket(TEXT("hand_l"));
	auto CurWeapon = GetWorld()->SpawnActor<ACharacterWeapon>(FVector(-10.935351f, 3.138973f, 3.343325f), FRotator(-14.627683f, -135.755875f, 97.12557f));
	auto CurWeapon2 = GetWorld()->SpawnActor<ACharacterSubWeapon>(FVector(-10.935351f, 3.138973f, 3.343325f), FRotator(-14.627683f, -135.755875f, 97.12557f));

	
	if (nullptr != CurWeapon) {
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		CurWeapon->SetActorRelativeLocation((FVector(-10.935351f, 3.138973f, 3.343325f)));
		CurWeapon->SetActorRelativeRotation((FRotator(-14.627683f, -135.755875f, 97.12557f)));
	}

	if (nullptr != CurWeapon2) {
		CurWeapon2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SubWeaponSocket);
		CurWeapon2->SetActorRelativeLocation((FVector(9.204125f, -2.534932f, 1.773366f)));
		CurWeapon2->SetActorRelativeRotation((FRotator(-36.767365, 46.686306f, -150.258850f)));
	}

}

void APlayerCharacter::Evade() {
	ABAnim->PlayEvadeMontage();
}

void APlayerCharacter::Run() {

		isRun = !isRun;

		if (isRun)
			GetCharacterMovement()->MaxWalkSpeed *= 3.0f;
		else
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;

		
}

void APlayerCharacter::MoveForward(float Value)
{
	
	ABAnim->ForwardDelta = Value;

	if ((Value != 0.0f) && !IsAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		

	//	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::White, FString::SanitizeFloat(Value));
		AddMovementInput(Direction, Value);
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}

	GEngine->AddOnScreenDebugMessage(2, 1.0f, FColor::Green, FString::SanitizeFloat(Value));

}

void APlayerCharacter::MoveRight(float Value)
{

	ABAnim->RightDelta = Value;

	if ((Controller != NULL) && (Value != 0.0f) && !isRun && !IsAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
		//GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Blue, FString::SanitizeFloat(CurrentXSpeed));
		//GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Yellow, isRun ? "True" : "False");
		AddMovementInput(Direction, Value);
	}
}

void APlayerCharacter::ViewTurnRight(float NewAxisValue)
{

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		AddControllerYawInput(NewAxisValue);

		//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
		//if (!IsAttacking)
		//{
		//	SetActorRelativeRotation(YawRotation);
		//}
		//Controller->SetControlRotation(YawRotation);
		

}

void APlayerCharacter::ViewTurnUp(float NewAxisValue)
{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		AddControllerPitchInput(NewAxisValue);
		//AddActorLocalRotation(YawRotation);
	
}


bool APlayerCharacter::CanSetWeapon()
{
	return (nullptr == CurrentWeapon);
}

void APlayerCharacter::SetWeapon(ACharacterWeapon * NewWeapon)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FName WeaponSocket(TEXT("hand_r"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void APlayerCharacter::SetWeapon(ACharacterWeapon * NewWeapon, ACharacterWeapon* NewWeapon2)
{
	ABCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon && nullptr != NewWeapon2 && nullptr == SubWeapon);
	FName WeaponSocket(TEXT("hand_r"));
	FName SubWeaponSocet(TEXT("hand_l"));
	if (nullptr != NewWeapon && nullptr != NewWeapon2)
	{
		NewWeapon2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SubWeaponSocet);
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		
		NewWeapon->SetOwner(this);
		NewWeapon->SetOwner(this);
		
		CurrentWeapon = NewWeapon;
		SubWeapon = NewWeapon2;
	}
}


void APlayerCharacter::BeginAttack()
{
	isAttack = true;
	isMoveable = false;
//	float t = Cast<UCharacterAnimInstance>(GetMesh()->GetAnimInstance()).time
	GetWorldTimerManager().SetTimer(AttackDelay_, this, &APlayerCharacter::StopAttack, 1.0f, false);
}

void APlayerCharacter::StopAttack()
{
	isAttack = false;
	isMoveable = true;
	
//	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "StopAttack");
}
void APlayerCharacter::Attack()
{
	
	if (IsAttacking)
	{
		GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Red, "CANT ATTACK");
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{

		
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();
		
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),	Camera->GetComponentLocation() + (Camera->GetForwardVector() * 1000));
		rot.Pitch= 0;
		SetActorRotation(rot);

		GEngine->AddOnScreenDebugMessage(4, 0.3f, FColor::Red, "ATTACK");
		
		IsAttacking = true;
		isAttack = true;
		GEngine->AddOnScreenDebugMessage(4, 0.3f, FColor::Red, FString::FormatAsNumber(IsAttacking));

		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
	}


	
}

void APlayerCharacter::Skill01() {


	if (IsAttacking)
	{
		GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Red, "CANT SKILL");
		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		ABCHECK(CurrentCombo == 0);
		AttackStartComboState();

		GEngine->AddOnScreenDebugMessage(6, 0.3f, FColor::Red, "SKILL");

		ABAnim->PlaySkillMontage();
		ABAnim->JumpToSkillMontageSection(CurrentCombo);
		IsAttacking = true;
		isAttack = true;
		GEngine->AddOnScreenDebugMessage(4, 0.3f, FColor::Red, FString::FormatAsNumber(IsAttacking));


	}
}

void APlayerCharacter::Skill02()
{
	
	float KnockDist = 500.0f;
	
	GEngine->AddOnScreenDebugMessage(8, 2.0f, FColor::Black, "RAY");
	
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (GetActorForwardVector() + -GetActorRightVector()) * KnockDist, FColor::Green, false, 2.0f, 0, 3.0f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() +(GetActorForwardVector()*500), FColor::Red, false, 2.0f, 0, 3.0f);
	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (GetActorForwardVector() + GetActorRightVector()) * 500, FColor::Blue, false, 2.0f, 0, 3.0f);


	switch (CurrentCombo) {
		case 0:
			GetWorld()->LineTraceMultiByChannel(SK2_SingleContainer1, GetActorLocation(), GetActorLocation() + (GetActorForwardVector() + -GetActorRightVector()) * KnockDist,ECollisionChannel::ECC_Pawn);
			GetWorld()->LineTraceMultiByChannel(SK2_SingleContainer2, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * KnockDist, ECollisionChannel::ECC_Pawn);
			GetWorld()->LineTraceMultiByChannel(SK2_SingleContainer3, GetActorLocation(), (GetActorForwardVector() + GetActorRightVector()) * 500, ECollisionChannel::ECC_Pawn);


			TArray<FHitResult> HitResult;
			FCollisionQueryParams Params(NAME_None, false, this);
			bool bResult = GetWorld()->SweepMultiByChannel(
				HitResult,
				GetActorLocation(),
				GetActorLocation() + GetActorForwardVector() * KnockDist,
				FQuat::Identity,
				ECollisionChannel::ECC_GameTraceChannel2,
				FCollisionShape::MakeSphere(AttackRadius),
				Params);
			
			

			DrawDebugSphere(GetWorld(), GetActorLocation(), AttackRadius, 6, FColor::Cyan, false, 3.0f, 0, 4.0f);


			if (bResult)
			{
				if (HitResult[0].Actor.IsValid())
				{
					float DragDistance = 100.0f;

			//		HitResult[0].Actor.Get()->SetActorLocation(FMath::Lerp(HitResult[0].Actor.Get()->GetActorLocation(), GetActorLocation() + (GetActorForwardVector()*DragDistance, 0.2f)));
					
					HitShake();
					//			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

					FDamageEvent DamageEvent;
					
					for (int i = 0; i < HitResult.Max(); i++) {
						HitResult[i].Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
					}
				}
			}

			break;
	}
}

void APlayerCharacter::HitShake() {
	FTimerHandle time_;
	if (CameraShake != nullptr)
	{
		GetWorld()->GetFirstPlayerController()->ClientPlayCameraShake(CameraShake, 1);
	}
	if (ForceFeedBack != nullptr) {
		GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ForceFeedBack, false, "Pawn");
	}

	AudioComp->Play();


	//Camera->PostProcessSettings.DepthOfFieldScale = 0.5f;
	Camera->PostProcessSettings.SceneFringeIntensity = 5.0f;
	GetWorldTimerManager().SetTimer(time_, this, &APlayerCharacter::HitPostProcess, 0.5f, false);


}

void APlayerCharacter::HitPostProcess() {
	Camera->PostProcessSettings.SceneFringeIntensity = 0.0f;
	Camera->PostProcessSettings.DepthOfFieldScale = 0.0f;

}

void APlayerCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	GEngine->AddOnScreenDebugMessage(9, 3.0f, FColor::Orange,"Still Ending");
		ABCHECK(isAttack);
		ABCHECK(IsAttacking);
		ABCHECK(CurrentCombo > 0);
		IsAttacking = false;
		isAttack = false;
		AttackEndComboState();
		
}

void APlayerCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void APlayerCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
	IsAttacking = false;
}

void APlayerCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			HitShake();
//			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
		}
	}
}

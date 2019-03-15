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
	SpringArm->TargetArmLength = 500;

	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(0, 0, 50));
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;

	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = true;
//	Camera->PostProcessSettings.GrainIntensity = 5.0f;

	Camera->PostProcessSettings.ChromaticAberrationStartOffset = 0.0f;
	//Camera->PostProcessSettings.ChromaticAberrationStartOffset = 10.0f;
//	FPostProcessSettings::BloomDirtMaskIntensity;
		// 5.0 ~ 0

	GetCharacterMovement()->bUseControllerDesiredRotation = true;
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
	ABAnim->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnDashMontageEnded);
	ABAnim->OnNextAttackCheck.AddLambda([this]() -> void {

		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			ABAnim->JumpToAttackMontageSection(CurrentCombo);
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack);
//	PlayerInputComponent->BindAction("Attack", IE_Released, this, &APlayerCharacter::StopAttack);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &APlayerCharacter::Run);

	PlayerInputComponent->BindAction("CheckFDash", IE_Pressed, this, &APlayerCharacter::CheckFDash);
	PlayerInputComponent->BindAction("CheckBDash", IE_Pressed, this, &APlayerCharacter::CheckBDash);
	PlayerInputComponent->BindAction("CheckRDash", IE_Pressed, this, &APlayerCharacter::CheckRDash);
	PlayerInputComponent->BindAction("CheckLDash", IE_Pressed, this, &APlayerCharacter::CheckLDash);

	PlayerInputComponent->BindAction("SwapWeapon_01", IE_Pressed, this, &APlayerCharacter::SwapWeapon_01);
	PlayerInputComponent->BindAction("SwapWeapon_02", IE_Pressed, this, &APlayerCharacter::SwapWeapon_02);

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


void APlayerCharacter::CheckFDash() {

	FDashStack++;

	if (FDashStack >= 2)
		ABAnim->PlayFDashSequence();
	GetWorldTimerManager().SetTimer(DashTimer_, this, &APlayerCharacter::FDash, 0.3f, false);

}

void APlayerCharacter::CheckBDash() {

	BDashStack++;
	if (BDashStack >= 2)
		ABAnim->PlayBDashSequence();
	

	GetWorldTimerManager().SetTimer(DashTimer_, this, &APlayerCharacter::BDash, 0.3f, false);

}
void APlayerCharacter::CheckRDash() {

	RDashStack++;
	if (RDashStack >= 2)
		ABAnim->PlayRDashSequence();
	GetWorldTimerManager().SetTimer(DashTimer_, this, &APlayerCharacter::RDash, 0.3f, false);

}
void APlayerCharacter::CheckLDash() {

	LDashStack++;
	if (LDashStack >= 2)
		ABAnim->PlayLDashSequence();
	GetWorldTimerManager().SetTimer(DashTimer_, this, &APlayerCharacter::LDash, 0.3f, false);

} 

void APlayerCharacter::FDash() {

	FDashStack = 0;
	isDashing = true;
	//GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Magenta, FString::SanitizeFloat(DashStack));
}

void APlayerCharacter::BDash() {
	
	BDashStack = 0;
	isDashing = true;
	//GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Magenta, FString::SanitizeFloat(DashStack));
}

void APlayerCharacter::RDash() {
	RDashStack = 0;
	isDashing = true;
	//GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Magenta, FString::SanitizeFloat(DashStack));
}

void APlayerCharacter::LDash() {
	LDashStack = 0;
	isDashing = true;
	//GEngine->AddOnScreenDebugMessage(5, 5.0f, FColor::Magenta, FString::SanitizeFloat(DashStack));
}

void APlayerCharacter::Run() {
	
	if (isWalk) {
		isRun = !isRun;

		if (isRun)
			GetCharacterMovement()->MaxWalkSpeed *= 3.0f;
		else
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;

		
		//GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Cyan, FString::SanitizeFloat(1.0f));
	}
}

void APlayerCharacter::MoveForward(float Value)
{
	

	if ((Controller != NULL) && (Value != 0.0f) && !IsAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		

	//	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::White, FString::SanitizeFloat(Value));
		AddMovementInput(Direction, Value);

		if(Value >= 1.0f)
			isWalk = true;
		else {
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
			isWalk = false;
			isRun = false;
		}
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}


}

void APlayerCharacter::MoveRight(float Value)
{
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
	
		if (!IsAttacking)
		{
			SetActorRelativeRotation(YawRotation);
		}
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
	//	GEngine->AddOnScreenDebugMessage(5, 3.0f, FColor::Red, "CANT ATTACK");
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
		
	//	GEngine->AddOnScreenDebugMessage(4, 3.0f, FColor::Red, "ATTACK");
		ABAnim->PlayAttackMontage();
		ABAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
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
	//GEngine->AddOnScreenDebugMessage(7, 3.0f, FColor::Orange,FString::SanitizeFloat(CurrentCombo));
		ABCHECK(IsAttacking);
		ABCHECK(CurrentCombo > 0);
		IsAttacking = false;
		AttackEndComboState();
		
}

void APlayerCharacter::OnDashMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	//GEngine->AddOnScreenDebugMessage(7, 3.0f, FColor::Orange, IsAttacking ? "IsAttack Montage END : TRUE" : "IsAttack Montage END : FALSE");
	
	isDashing = false;


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

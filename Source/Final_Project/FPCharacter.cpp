// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacter.h"
#include "FPAnimInstance.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPCharacter::AFPCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// ACharacter Class�� APawn�� ��ӹ��� Ŭ������ ������ ĳ������ �⺻���� ������Ʈ���� �̹� �߰��Ǿ� ����.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINTARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	// ĸ�� �ݶ��̴� ����� �κ� (������, APawn�� �޸� ACharacter���� �ڵ� ������ �ȴ�.)
	//GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	//GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	// �޽� �߽��� �� ����(Ʈ������ ��ġ)
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	// �������� �Ÿ� �� �����̼� ���� (ī�޶�)
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// �޽��� �ڵ�� �ҷ��� �������ֱ�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Warrior.SK_CharM_Warrior"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}


	// �ִϸ��̼� �������Ʈ ����ϱ�
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// �ִϸ��̼� �������Ʈ�� Ŭ����(�ִ� �ν��Ͻ�)�̱� ������ ��� ���� "_C"�� �߰��� �ٿ���� �Ѵ�.
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	SetControlMode(EControlMode::GTA);

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;
	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;

	MaxCombo = 4;
	AttackEndComboState();

	AttackRange = 200.0f;
	AttackRadius = 50.0f;

	// ������Ʈ ���� -> �ݸ������� �����¿� FPCharacter�̶�� �̸��� ã�� �� ���������� �ݸ����� �����Ѵ�.
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("FPCharacter"));

}

// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

}



void AFPCharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	// 3��Ī ���� ��� (GTA ���)
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;

		// ĳ���Ͱ� �����̴� �������� ���� ���ֱ�
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;

		// Ű���� �Է½� ĳ������ �ε巯�� ȸ�� ����
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	}


}

// Called every frame
void AFPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FMath::FInterpTo --> ��ǥ �������� �ε巴�� �����̱�
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		//SpringArm->AddRelativeRotation = FMath::RInterpTo(SpringArm->AddRelativeRotation, ArmRotationTo, DeltaTime, ArmRotationSpeed);
		break;
	}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}
}

void AFPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// ��������Ʈ ����� ���� �ִ��ν��Ͻ��� OnMontageEnded ��������Ʈ�� ���ε��Ѵ�. 
	FPAnim = Cast<UFPAnimInstance>(GetMesh()->GetAnimInstance());
	FPCHECK(nullptr != FPAnim);

	FPAnim->OnMontageEnded.AddDynamic(this, &AFPCharacter::OnAttackMontageEnded); // �� �Լ��� ����Ǹ�, ��Ÿ�ְ� ������ �� ���⿡ �̷������� ���ε��� ��� �Լ����� ����ȴ�.

	// �̷��� �����ϸ� ��Ÿ�ֿ��� NextAttackCheck ��Ƽ���̰� ���� �Ǿ��� ��, �Ʒ� ���� ������ �ߵ��ȴ�.
	FPAnim->OnNextAttackCheck.AddLambda([this]() -> void {
		FPLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			FPAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});

	FPAnim->OnAttackHitCheck.AddUObject(this, &AFPCharacter::AttackCheck);
}

float AFPCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	FPLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	if (FinalDamage > 0.0f)
	{
		FPAnim->SetDeadAnim();
		SetActorEnableCollision(false);
	}

	return FinalDamage;
}

// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AFPCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AFPCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AFPCharacter::Attack);

	// Axis ���� Ȱ���Ͽ� �̵�Ű �����
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AFPCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AFPCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFPCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFPCharacter::Turn);


}

void AFPCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// ���� ĳ���Ͱ� ���� �������, ����� ��Ʈ�ѷ��� ���� �������, �Ʒ��� ��Ʈ�ѷ��� ���� ���⿡�� Z��� ������ ����.
		//AddMovementInput(GetActorForwardVector(), NewAxisValue);
		//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}
}

void AFPCharacter::LeftRight(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}
}

// Y�� ȸ�� (Pitch)
void AFPCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}

// Z�� ȸ�� (Yaw)
void AFPCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
}

void AFPCharacter::ViewChange()
{
	FPLOG_S(Warning);
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		//GetController()->SetControlRotation(SpringArm->AddRelativeRotation);
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AFPCharacter::Attack()
{
	//if (IsAttacking) return;

	//FPAnim->PlayAttackMontage();
	//IsAttacking = true;

	if (IsAttacking)
	{
		FPCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		FPCHECK(CurrentCombo == 0);
		AttackStartComboState();
		FPAnim->PlayAttackMontage();
		FPAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}
}

void AFPCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	//IsAttacking = false;

	FPCHECK(IsAttacking);
	FPCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
}

void AFPCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	FPCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AFPCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AFPCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	// �÷��̾� ������ 200cm��ŭ ���鼭 �浹ü�� �ִ��� Ž��
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,	// �̰��� Attack Ʈ���̽� ä���� �ǹ��Ѵ�. �ݸ��� ä���� ���� �߰��ϸ� ������Ʈ��/Config/DefaultEngine.ini���� ��ϵ� ä�� ������ŭ ECC_GameTraceChannel�� ���� �� ������ ������� �� �� �ִ�.
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	// �� �κ��� ����� ��������� ����� ȯ�濡�� ���� ������ ������ ���̰� ���ش�.
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);

#endif
	
	// ������ �¾Ҵ��� üũ�ϴ� �κ�
	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			FPLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);
		}
	}

}


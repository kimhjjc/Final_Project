// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/FPCharacter.h"
#include "Characters/Player/FPAnimInstance.h"
#include "Item/FPWeapon.h"
#include "Characters/Player/FPCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
// �� �������(Components/WidgetComponent.h)�� UI�� �ٷ�� ���� �߰��� �����̴�.
// ���� ������Ʈ�� �ƴ� UE4 ������Ʈ�� �������������, Final_Project.Build.cs���� "Core", "CoreUObject", "Engine", "InputCore" ��� ���� "UMG"�� �߰��� �־��� ������ �����ο� ����� ����������.
#include "Components/WidgetComponent.h"	
#include "UI/FPCharacterWidget.h"
#include "UI/FPHUDWidget.h"
#include "UI/FPQuestWidget.h"
#include "Characters/FPAIController.h"
#include "Characters/Player/FPPlayerController.h"
#include "FPCharacterSetting1.h"
#include "FPGameInstance.h"
#include "FPPlayerState.h"
#include "FPGameMode.h"
#include "Effect/FPHitEffect.h"
#include "Effect/FPHitPunchEffect.h"
#include "Inventory/Item.h"
#include "Inventory/InventoryComponent.h"

// Sets default values
AFPCharacter::AFPCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Inventory = CreateDefaultSubobject<UInventoryComponent>("Inventory");
	Inventory->Capacity = 20;

	Health = 100.f;

	// ACharacter Class�� APawn�� ��ӹ��� Ŭ������ ������ ĳ������ �⺻���� ������Ʈ���� �̹� �߰��Ǿ� ����.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINTARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UFPCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

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

	IsRestEntered = false;
	IsResting = false;
	IsAttacking = false;
	IsAnimFinish = true;

	MaxCombo = 4;
	AttackEndComboState();

	AttackRange = 80.0f;
	AttackRadius = 50.0f;

	IsAnimMotionMoveing = false;
	bIsNPCInteractive = false;

	// ������Ʈ ���� -> �ݸ������� �����¿� FPCharacter�̶�� �̸��� ã�� �� ���������� �ݸ����� �����Ѵ�.
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("FPCharacter"));

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));	// ��� �����ͷ� �����Ͽ����Ƿ� ��� �ڿ� _C�� �ٿ��־�� �Ѵ�.
	if (UI_HUD.Succeeded())
	{
		// UI_HUD.Class�� �ᱹ UI_HPBar�� ��������Ϳ��� �׷���->Ŭ���� ���� -> Ŭ�����ɼ��� �θ�Ŭ������ ������ ���� �ǹ��Ѵ�.
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	// �� �� ������ �⺻ ������, Pawnī�װ����� �� �� �ִ�.
	AIControllerClass = AFPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;	// AI�� �����Ǵ� �ɼ�

	// GetDefault �Լ��� ���� �޸𸮿� �ö� Ŭ���� �⺻ ��ü�� ������ �� �ִ�.
	auto DefaultSetting = GetDefault<UFPCharacterSetting1>();
	if (DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharacterAsset : DefaultSetting->CharacterAssets)
		{
			FPLOG(Warning, TEXT("Character Asset : %s"), *CharacterAsset.ToString());
		}
	}


	AssetIndex = 4;

	SetActorHiddenInGame(true);
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);
	// bCanBeDamaged = false;	 // �𸮾� ������ ��ȭ�� ���� private���� �ٲ�� SetCanBeDamged�� �̿��ؾ��� (Actor Class).

	DeadTimer = 5.0f;
}

void AFPCharacter::UseItem(UItem * Item)
{
	if (Item)
	{
		Item->Use(this);
		Item->OnUse(this); //bp event
	}
}

void AFPCharacter::SetCharacterState(ECharacterState NewState)
{
	FPCHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (CurrentState)
	{
	case ECharacterState::LOADING:
	{
		if (bIsPlayer)
		{
			DisableInput(FPPlayerController);

			FPPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);

			// CharacterState�� �̹� PlayerController�� PostLogin���� �ʱ�ȭ�� ��ġ�Ƿ� �������⸸ �ϸ� �ȴ�.
			auto FPPlayerState = Cast<AFPPlayerState>(GetPlayerState());
			FPCHECK(nullptr != FPPlayerState);
			CharacterStat->SetNewLevel(FPPlayerState->GetCharacterLevel());
		}
		else
		{
			auto FPGameMode = Cast<AFPGameMode>(GetWorld()->GetAuthGameMode());
			FPCHECK(nullptr != FPGameMode);
			//int32 TargetLevel = FMath::CeilToInt(((float)FPGameMode->GetScore() * 0.8f));
			int32 TargetLevel = 2;
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			FPLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			CharacterStat->SetNewLevel(FinalLevel);

		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);

		CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
			SetCharacterState(ECharacterState::DEAD);
			});

		auto CharacterWidget = Cast<UFPCharacterWidget>(HPBarWidget->GetUserWidgetObject());
		FPCHECK(nullptr != CharacterWidget);
		CharacterWidget->BindCharacterStat(CharacterStat);

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::GTA);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(FPPlayerController);

			auto FPPlayerState = Cast<AFPPlayerState>(GetPlayerState());
			FPCHECK(nullptr != FPPlayerState);
			FPPlayerController->GetFPPlayerState()->OnLevelUpDelegate.AddLambda([this]() -> void {
				CharacterStat->SetNewLevel(FPPlayerController->GetFPPlayerState()->GetCharacterLevel());
				});
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 300.0f;
			FPAIController->RUNAI(this);
		}

		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		//SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		FPAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			DisableInput(FPPlayerController);
		}
		else
		{
			FPAIController->StopAI();
		}

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
			if (bIsPlayer)
			{
				FPPlayerController->RestartLevel();
			}
			else
			{
				Destroy();
			}
			}), DeadTimer, false);

		break;
	}
	}
}

ECharacterState AFPCharacter::GetCharacterState() const
{
	return CurrentState;
}

int32 AFPCharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

float AFPCharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

float AFPCharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ? (CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : CharacterStat->GetAttack();
	float AttackModifier = (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackModifier() : 1.0f;

	return AttackDamage * AttackModifier;
}

// Called when the game starts or when spawned
void AFPCharacter::BeginPlay()
{
	Super::BeginPlay();

	bIsPlayer = IsPlayerControlled();
	if (bIsPlayer)
	{
		FPPlayerController = Cast<AFPPlayerController>(GetController());
		FPCHECK(nullptr != FPPlayerController);
	}
	else
	{
		FPAIController = Cast<AFPAIController>(GetController());
		FPCHECK(nullptr != FPAIController);
	}

	auto DefaultSetting = GetDefault<UFPCharacterSetting1>();

	if (bIsPlayer)
	{
		AssetIndex = 4;
	}
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto FPGameInstance = Cast<UFPGameInstance>(GetGameInstance());
	FPCHECK(nullptr != FPGameInstance);
	AssetStreamingHandle = FPGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AFPCharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);

	/*
	// ������ �ʱ�ȭ ������ PostInitializeComponents�̾�����, �𸮾� 4.21 �������� BeginPlay�� ����Ǿ� ������ ��� �� ������ �ʱ�ȭ�� �������ش�.
	// ���� BeginPlay ������ ȣ��Ǵ� PostInitializeComponents���� �߻��� UI���� ����� ��� �ݿ����� �ʴ´�.
	auto CharacterWidget = Cast<UFPCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}




	// FPGameInstance�� FStreamablemanger�� ���� ���� �޽� �������ֱ�. (�÷��̾� ����)
	if (!IsPlayerControlled())
	{
		auto DefaultSetting = GetDefault<UFPCharacterSetting1>();
		int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
		CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];

		auto FPGameInstance = Cast<UFPGameInstance>(GetGameInstance());
		if (nullptr != FPGameInstance)
		{
			AssetStreamingHandle = FPGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AFPCharacter::OnAssetLoadCompleted));
			//AssetStreamingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AABCharacter::OnAssetLoadCompleted));
		}
	}
	*/


	// ���⸦ ���ͷ� �����Ͽ� �����տ� FPCharacter�� �θ�� �����ϴ� ���.
	/*
	FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AFPWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
	*/

	// �̰� ���� ���͸� �����ϴ°� �ƴ϶� �޽��� �����տ� ���� ������Ʈ�� �������ִ� ���.
	//FName WeaponSocket(TEXT("hand_rSocket"));
	//if (GetMesh()->DoesSocketExist(WeaponSocket))
	//{
	//	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	//	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SK_Blade_HeroSword11.SK_Blade_HeroSword11"));
	//	if (SK_WEAPON.Succeeded())
	//	{
	//		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	//	}

	//	Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	//}

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
	case EControlMode::NPC:
		// NPC�� ������ AI�� ���� ���� ����.
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

	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	if (FVector::Dist(GetActorLocation(), PlayerLocation) <= 3000)
		HPBarWidget->SetHiddenInGame(false);
	else
		HPBarWidget->SetHiddenInGame(true);


	if (IsRestEntered)
		if (IsResting)
		{
			CharacterStat->SetHeal(0.1f);
		}

	// ���� �ִϸ��̼ǿ��� �����̴� �κ�
	MovementAnimMotionState();

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

	FPAnim->OnMontageStarted.AddDynamic(this, &AFPCharacter::OnAttackMontageStarted);
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
		else if (IsRMBComboInputOn)
		{
			AttackStartComboState();
			FPAnim->PlayAttack_RMBMontage();
		}
		});

	FPAnim->OnAttackHitCheck.AddUObject(this, &AFPCharacter::AttackCheck);

	FPAnim->OnMotionBasedMovement.AddLambda([this]() -> void {
		FPLOG(Warning, TEXT("OnMotionBasedMovement"));
		IsAnimMotionMoveing = true;
		});

	FPAnim->OnMotionBasedMovementFinish.AddLambda([this]() -> void {
		FPLOG(Warning, TEXT("OnMotionBasedMovement"));
		IsAnimMotionMoveing = false;
		});

	FPAnim->OnFinishMontage.AddLambda([this]() -> void {
		FPLOG(Warning, TEXT("OnAnimFinish"));
		IsAnimFinish = true;
		});

	FPAnim->OnRest_Looping.AddLambda([this]() -> void {
		FPLOG(Warning, TEXT("OnRest_Looping"));
		IsResting = true;
		});

	FPAnim->OnRest_Finish.AddLambda([this]() -> void {
		FPLOG(Warning, TEXT("OnRest_Finish"));
		IsRestEntered = false;
		});


	CharacterStat->OnDamagedHP.AddLambda([this]() -> void {
		if (bIsPlayer)
			ActFinish();
		});


	//auto FPPlayerState = Cast<AFPPlayerState>(GetPlayerState());
	//FPCHECK(nullptr != FPPlayerState);
	//FPPlayerController->GetFPPlayerState()->OnLevelUpDelegate.AddLambda([this]() -> void {
	//	CharacterStat->SetNewLevel(FPPlayerController->GetFPPlayerState()->GetCharacterLevel());
	//	});
	// �� �κ��� SetCharacterState�� DEAD���� ��� ó���ϰ� �ȴ�.
	//CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
	//	FPLOG(Warning, TEXT("OnHPIsZero"));
	//	FPAnim->SetDeadAnim();
	//	SetActorEnableCollision(false);
	//	});
}

float AFPCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	FPLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);
	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())
		{
			auto FPPlayerController1 = Cast<AFPPlayerController>(EventInstigator);
			FPCHECK(nullptr != FPPlayerController1, 0.0f);
			FPPlayerController1->NPCKill(this);
		}
	}

	return FinalDamage;
}

void AFPCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

}

// Called to bind functionality to input
void AFPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Action ����
	PlayerInputComponent->BindAction(TEXT("ActFinish"), EInputEvent::IE_Pressed, this, &AFPCharacter::ActFinish);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AFPCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AFPCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AFPCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack_RMB"), EInputEvent::IE_Pressed, this, &AFPCharacter::Attack_RMB);
	PlayerInputComponent->BindAction(TEXT("Rest"), EInputEvent::IE_Pressed, this, &AFPCharacter::Skill_Rest);
	PlayerInputComponent->BindAction(TEXT("Quest_Open"), EInputEvent::IE_Pressed, this, &AFPCharacter::Quest_Open);

	// Axis ���� Ȱ���Ͽ� �̵�Ű �����
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AFPCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AFPCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AFPCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AFPCharacter::Turn);


}

bool AFPCharacter::CanSetWeapon()
{
	//return (nullptr == CurrentWeapon);
	return true;
}

void AFPCharacter::Setweapon(AFPWeapon * NewWeapon)
{
	//FPCHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);
	FPCHECK(nullptr != NewWeapon);

	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
	FName WeaponSocket(TEXT("hand_rSocket"));
	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}

}

void AFPCharacter::UpDown(float NewAxisValue)
{
	if (IsActing())
		return;

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
	if (IsActing())
		return;

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

void AFPCharacter::Skill_Rest()
{
	FPCHECK(!IsActing());

	IsRestEntered = true;
	FPAnim->PlayRest_Montage();
}

void AFPCharacter::ActFinish()
{
	// ���߿� Rest���� �ൿ�� �ߴ��ؾ� �ϴ� ���� ������ ���.
	if (IsResting)
		FinishResting();
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

void AFPCharacter::Quest_Open()
{
	auto NPCQuestWidget = FPPlayerController->GetQuestWidget();
	FPCHECK(nullptr != NPCQuestWidget);
	// NPCQuestWidget->SetIsEnabled(!NPCQuestWidget->GetIsEnabled());
	if (NPCQuestWidget->GetVisibility() == ESlateVisibility::Visible)
		NPCQuestWidget->SetVisibility(ESlateVisibility::Collapsed);
	else
		NPCQuestWidget->SetVisibility(ESlateVisibility::Visible);
}


void AFPCharacter::Attack()
{
	FPCHECK(!IsActing() || IsAttacking);

	if (IsAttacking)
	{
		FPCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)
		{
			IsComboInputOn = true;
			IsRMBComboInputOn = false;
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

void AFPCharacter::Attack_RMB()
{

	FPCHECK(!IsActing() || IsAttacking);

	// �⺻ ���� �޺� �� RMB�� �� 4ȸ �޺� �� CurrentCombo�� 0�� 2�϶��� �� �� �ִ�.
	if (!(CurrentCombo == 0 || CurrentCombo == 2))
		return;

	if (IsAttacking)
	{
		IsRMBComboInputOn = true;
		IsComboInputOn = false;
	}
	else
	{
		AttackStartComboState();
		FPAnim->PlayAttack_RMBMontage();
		IsAttacking = true;
	}
}

void AFPCharacter::OnAttackMontageStarted(UAnimMontage * Montage)
{
	IsAnimFinish = false;
}

void AFPCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	//IsAttacking = false;
	//FPCHECK(IsAnimFinish);
	if (!IsAnimFinish)	return;

	IsAnimFinish = false;

	FPCHECK(IsAttacking);
	FPCHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();
	OnAttackEnd.Broadcast();
}

void AFPCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	IsRMBComboInputOn = false;
	FPCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AFPCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	IsRMBComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AFPCharacter::AttackCheck()
{
	float FinalAttackRange = GetFinalAttackRange();

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	// �÷��̾� ������ 200cm��ŭ ���鼭 �浹ü�� �ִ��� Ž��
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,	// �̰��� Attack Ʈ���̽� ä���� �ǹ��Ѵ�. �ݸ��� ä���� ���� �߰��ϸ� ������Ʈ��/Config/DefaultEngine.ini���� ��ϵ� ä�� ������ŭ ECC_GameTraceChannel�� ���� �� ������ ������� �� �� �ִ�.
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	// �� �κ��� ����� ��������� ����� ȯ�濡�� ���� ������ ������ ���̰� ���ش�.
#if ENABLE_DRAW_DEBUG
	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
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

			auto SpawnLocation = HitResult.Actor->GetActorLocation() + FVector(rand() % 70 - 35, rand() % 70 - 35, rand() % 30 - 15);
			if (bIsPlayer && CurrentWeapon)
			{
				auto SpawnedHitEffect = GetWorld()->SpawnActor<AFPHitEffect>(AFPHitEffect::StaticClass(), SpawnLocation, FRotator(rand() % 360, rand() % 360, rand() % 360));
			}
			else
			{
				auto SpawnedHitEffect = GetWorld()->SpawnActor<AFPHitPunchEffect>(AFPHitPunchEffect::StaticClass(), SpawnLocation, FRotator(rand() % 360, rand() % 360, rand() % 360));
			}
			FDamageEvent DamageEvent;
			//HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}

}

void AFPCharacter::MovementAnimMotionState()
{
	if (!IsAttacking || !IsAnimMotionMoveing)
		return;

	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), 1);
}

void AFPCharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	FPCHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);

	SetCharacterState(ECharacterState::READY);
}

bool AFPCharacter::IsActing()
{
	if (IsAttacking || IsResting || IsRestEntered)
		return true;

	return false;
}

void AFPCharacter::FinishResting()
{
	// �� �Լ��� �� ��
	// ����Ű(�¿�, �յ�), ����(LMB,RMB), ���߿� ���� �¾��� ��, �׸��� ���� ��ȣ�ۿ��
	IsResting = false;
	FPAnim->JumpToRest_End_MontageSection();
}


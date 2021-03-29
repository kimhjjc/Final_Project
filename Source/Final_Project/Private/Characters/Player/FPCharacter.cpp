// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/FPCharacter.h"
#include "Characters/Player/FPAnimInstance.h"
#include "Item/FPWeapon.h"
#include "Characters/Player/FPCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
// 이 헤더파일(Components/WidgetComponent.h)은 UI를 다루기 위해 추가한 파일이다.
// 현재 프로젝트가 아닌 UE4 프로젝트의 헤더파일이지만, Final_Project.Build.cs에서 "Core", "CoreUObject", "Engine", "InputCore" 모듈 옆에 "UMG"를 추가해 주었기 때문에 자유로운 사용이 가능해진다.
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

	// ACharacter Class는 APawn를 상속받은 클래스로 폰보다 캐릭터의 기본적인 컴포넌트들이 이미 추가되어 있음.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINTARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UFPCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	// 캡슐 콜라이더 만드는 부분 (이지만, APawn과 달리 ACharacter에는 자동 설정이 된다.)
	//GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	//GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	// 메쉬 중심점 값 대입(트랜스폼 위치)
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	// 스프링암 거리 및 로테이션 조절 (카메라)
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// 메쉬를 코드로 불러서 설정해주기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Warrior.SK_CharM_Warrior"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}


	// 애니메이션 블루프린트 사용하기
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// 애니메이션 블루프린트는 클래스(애님 인스턴스)이기 때문에 경로 끝에 "_C"를 추가로 붙여줘야 한다.
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

	// 프로젝트 세팅 -> 콜리전에서 프리셋에 FPCharacter이라는 이름을 찾아 그 프리셋으로 콜리전을 변경한다.
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("FPCharacter"));

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));	// 블루 프린터로 제작하였으므로 경로 뒤에 _C를 붙여주어야 한다.
	if (UI_HUD.Succeeded())
	{
		// UI_HUD.Class가 결국 UI_HPBar가 블루프린터에서 그래프->클레스 세팅 -> 클래스옵션의 부모클래스로 설정한 것을 의미한다.
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	// 이 두 변수는 기본 변수로, Pawn카테고리에서 볼 수 있다.
	AIControllerClass = AFPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;	// AI가 생성되는 옵션

	// GetDefault 함수를 통해 메모리에 올라간 클래스 기본 객체를 가져올 수 있다.
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
	// bCanBeDamaged = false;	 // 언리얼 버전의 진화에 따라 private으로 바뀌어 SetCanBeDamged를 이용해야함 (Actor Class).

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

			// CharacterState는 이미 PlayerController의 PostLogin에서 초기화를 마치므로 가져오기만 하면 된다.
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
	// 위젯의 초기화 시점이 PostInitializeComponents이었지만, 언리얼 4.21 버전부터 BeginPlay로 변경되어 위젯의 경우 이 곳에서 초기화를 진행해준다.
	// 또한 BeginPlay 이전에 호출되는 PostInitializeComponents에서 발생한 UI관련 명령은 모두 반영되지 않는다.
	auto CharacterWidget = Cast<UFPCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	if (nullptr != CharacterWidget)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}




	// FPGameInstance의 FStreamablemanger를 통한 랜덤 메쉬 지정해주기. (플레이어 제외)
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


	// 무기를 액터로 제작하여 오른손에 FPCharacter를 부모로 지정하는 방법.
	/*
	FName WeaponSocket(TEXT("hand_rSocket"));
	auto CurWeapon = GetWorld()->SpawnActor<AFPWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	if (nullptr != CurWeapon)
	{
		CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
	}
	*/

	// 이건 무기 액터를 생성하는게 아니라 메쉬의 오른손에 직접 컴포넌트로 생성해주는 방법.
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

	// 3인칭 게임 모드 (GTA 방식)
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

		// 캐릭터가 움직이는 방향으로 보게 해주기
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

		// 키보드 입력시 캐릭터의 부드러운 회전 연출
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::NPC:
		// NPC로 설정된 AI가 가질 설정 조작.
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

	// 공격 애니메이션에서 움직이는 부분
	MovementAnimMotionState();

	//FMath::FInterpTo --> 목표 지점까지 부드럽게 움직이기
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
	// 델리게이트 사용을 위해 애님인스턴스의 OnMontageEnded 델리게이트에 바인딩한다. 
	FPAnim = Cast<UFPAnimInstance>(GetMesh()->GetAnimInstance());
	FPCHECK(nullptr != FPAnim);

	FPAnim->OnMontageStarted.AddDynamic(this, &AFPCharacter::OnAttackMontageStarted);
	FPAnim->OnMontageEnded.AddDynamic(this, &AFPCharacter::OnAttackMontageEnded); // 이 함수가 선언되면, 몽타주가 끝났을 때 여기에 이런식으로 바인딩한 모든 함수들이 실행된다.

	// 이렇게 선언하면 몽타주에서 NextAttackCheck 노티파이가 실행 되었을 때, 아래 람다 구문이 발동된다.
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
	// 이 부분은 SetCharacterState의 DEAD에서 대신 처리하게 된다.
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

	// Action 매핑
	PlayerInputComponent->BindAction(TEXT("ActFinish"), EInputEvent::IE_Pressed, this, &AFPCharacter::ActFinish);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &AFPCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AFPCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AFPCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack_RMB"), EInputEvent::IE_Pressed, this, &AFPCharacter::Attack_RMB);
	PlayerInputComponent->BindAction(TEXT("Rest"), EInputEvent::IE_Pressed, this, &AFPCharacter::Skill_Rest);
	PlayerInputComponent->BindAction(TEXT("Quest_Open"), EInputEvent::IE_Pressed, this, &AFPCharacter::Quest_Open);

	// Axis 매핑 활용하여 이동키 만들기
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
		// 위는 캐릭터가 보는 방향기준, 가운데는 컨트롤러가 보는 방향기준, 아래는 컨트롤러가 보는 방향에서 Z축과 평행한 기준.
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

// Y축 회전 (Pitch)
void AFPCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}

// Z축 회전 (Yaw)
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
	// 나중에 Rest같이 행동을 중단해야 하는 것이 나오면 사용.
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

	// 기본 공격 콤보 중 RMB는 총 4회 콤보 중 CurrentCombo가 0과 2일때만 들어갈 수 있다.
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
	// 플레이어 앞으로 200cm만큼 쓸면서 충돌체가 있는지 탐색
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,	// 이것은 Attack 트레이스 채널을 의미한다. 콜리전 채널을 따로 추가하면 프로젝트명/Config/DefaultEngine.ini에서 등록된 채널 개수만큼 ECC_GameTraceChannel이 생겨 그 파일을 열어봐야 알 수 있다.
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	// 이 부분은 디버그 드로잉으로 디버깅 환경에서 공격 범위를 눈으로 보이게 해준다.
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

	// 실제로 맞았는지 체크하는 부분
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
	// 이 함수가 들어간 곳
	// 방향키(좌우, 앞뒤), 공격(LMB,RMB), 나중에 공격 맞았을 때, 그리고 각종 상호작용들
	IsResting = false;
	FPAnim->JumpToRest_End_MontageSection();
}


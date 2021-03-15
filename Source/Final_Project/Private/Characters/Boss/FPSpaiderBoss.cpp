// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Boss/FPSpaiderBoss.h"
#include "Characters/Enemy/FPMonsterAnimInstance.h"
#include "Item/FPWeapon.h"
#include "Characters/Player/FPCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"	
#include "UI/FPCharacterWidget.h"
#include "Characters/FPAIController.h"
#include "FPCharacterSetting1.h"
#include "FPGameInstance.h"
#include "Characters/Player/FPPlayerController.h"
#include "UI/FPHUDWidget.h"
#include "FPGameMode.h"
#include "Effect/FPHitBossEffect.h"

// Sets default values
AFPSpaiderBoss::AFPSpaiderBoss()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterStat = CreateDefaultSubobject<UFPCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetCapsuleComponent()->SetCapsuleRadius(70.0f);

	// �޽��� �ڵ�� �ҷ��� �������ֱ�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SPAIDER_MESH(TEXT("/Game/Enemy/Enemy_Great_Spider/SK_Greater_Spider.SK_Greater_Spider"));
	if (SPAIDER_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SPAIDER_MESH.Object);
	}

	// �ִϸ��̼� �������Ʈ ����ϱ�
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// �ִϸ��̼� �������Ʈ�� Ŭ����(�ִ� �ν��Ͻ�)�̱� ������ ��� ���� "_C"�� �߰��� �ٿ���� �Ѵ�.
	static ConstructorHelpers::FClassFinder<UAnimInstance> BOSSMONSTER_ANIM(TEXT("/Game/Enemy/Enemy_Great_Spider/SpiaderBoss_BluePrint.SpiaderBoss_BluePrint_C"));
	if (BOSSMONSTER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BOSSMONSTER_ANIM.Class);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("FPCharacter"));
	GetMesh()->SetCollisionProfileName("NoCollision");

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

	IsAttacking = false;
	AttackRange = 100.0f;
	AttackRadius = 80.0f;

	IsDead = false;
	DeadTimer = 1.25f;
}

// Called when the game starts or when spawned
void AFPSpaiderBoss::BeginPlay()
{
	Super::BeginPlay();

	FPAIController = Cast<AFPAIController>(GetController());

	auto FPGameMode = Cast<AFPGameMode>(GetWorld()->GetAuthGameMode());
	FPCHECK(nullptr != FPGameMode);
	int32 TargetLevel = 3;
	int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
	FPLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
	CharacterStat->SetNewMonsterLevel(FinalLevel);

	auto CharacterWidget = Cast<UFPCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	FPCHECK(nullptr != CharacterWidget);
	CharacterWidget->BindCharacterStat(CharacterStat);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	FPAIController->RUNAI(this);

	CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
		IsDead = true;
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		FPMonsterAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		FPAIController->StopAI();
		OnHPIsZero.Broadcast();

		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
			Destroy();
			}), DeadTimer, false);
		});
}

// Called every frame
void AFPSpaiderBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSpaiderBoss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	FPMonsterAnim = Cast<UFPMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	FPCHECK(nullptr != FPMonsterAnim);

	FPMonsterAnim->OnMontageEnded.AddDynamic(this, &AFPSpaiderBoss::OnAttackMontageEnded); // �� �Լ��� ����Ǹ�, ��Ÿ�ְ� ������ �� ���⿡ �̷������� ���ε��� ��� �Լ����� ����ȴ�.

	FPMonsterAnim->OnAttackHitCheck.AddUObject(this, &AFPSpaiderBoss::AttackCheck);

}

float AFPSpaiderBoss::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	FPLOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);

	CharacterStat->SetDamage(FinalDamage);
	if (IsDead)
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

// Called to bind functionality to input
void AFPSpaiderBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFPSpaiderBoss::Attack()
{
	if (IsAttacking) return;

	FPLOG(Warning, TEXT("Boss Attack!"));
	FPMonsterAnim->PlayBossAttackMontage();
	IsAttacking = true;
}

void AFPSpaiderBoss::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	OnAttackEnd.Broadcast();
	IsAttacking = false;
}

void AFPSpaiderBoss::AttackCheck()
{
	//float FinalAttackRange = GetFinalAttackRange();
	float FinalAttackRange = AttackRange;

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
			auto SpawnedHitEffect = GetWorld()->SpawnActor<AFPHitBossEffect>(AFPHitBossEffect::StaticClass(), SpawnLocation, FRotator(rand() % 360, rand() % 360, rand() % 360));

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			//HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}
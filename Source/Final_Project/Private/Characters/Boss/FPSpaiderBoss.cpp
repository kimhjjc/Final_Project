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

	// 메쉬를 코드로 불러서 설정해주기
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SPAIDER_MESH(TEXT("/Game/Enemy/Enemy_Great_Spider/SK_Greater_Spider.SK_Greater_Spider"));
	if (SPAIDER_MESH.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SPAIDER_MESH.Object);
	}

	// 애니메이션 블루프린트 사용하기
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// 애니메이션 블루프린트는 클래스(애님 인스턴스)이기 때문에 경로 끝에 "_C"를 추가로 붙여줘야 한다.
	static ConstructorHelpers::FClassFinder<UAnimInstance> BOSSMONSTER_ANIM(TEXT("/Game/Enemy/Enemy_Great_Spider/SpiaderBoss_BluePrint.SpiaderBoss_BluePrint_C"));
	if (BOSSMONSTER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(BOSSMONSTER_ANIM.Class);
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("FPCharacter"));
	GetMesh()->SetCollisionProfileName("NoCollision");

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

	FPMonsterAnim->OnMontageEnded.AddDynamic(this, &AFPSpaiderBoss::OnAttackMontageEnded); // 이 함수가 선언되면, 몽타주가 끝났을 때 여기에 이런식으로 바인딩한 모든 함수들이 실행된다.

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
			auto SpawnedHitEffect = GetWorld()->SpawnActor<AFPHitBossEffect>(AFPHitBossEffect::StaticClass(), SpawnLocation, FRotator(rand() % 360, rand() % 360, rand() % 360));

			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
			//HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "FPMonster.h"
#include "FPMonsterAnimInstance.h"
#include "FPWeapon.h"
#include "FPCharacterStatComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"	
#include "FPCharacterWidget.h"
#include "FPAIController.h"
#include "FPCharacterSetting1.h"
#include "FPGameInstance.h"
#include "FPPlayerController.h"
#include "FPPlayerState.h"
#include "FPHUDWidget.h"
#include "FPGameMode.h"

// Sets default values
AFPMonster::AFPMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CharacterStat = CreateDefaultSubobject<UFPCharacterStatComponent>(TEXT("CHARACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	// �޽��� �ڵ�� �ҷ��� �������ֱ�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD1(TEXT("/Game/Enemy/Enemy_Gruntling_Guardian/SK_Exodus_Gruntling.SK_Exodus_Gruntling"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD2(TEXT("/Game/Enemy/Enemy_Gruntling_Guardian/SK_Gruntling_Avalanche.SK_Gruntling_Avalanche"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD3(TEXT("/Game/Enemy/Enemy_Gruntling_Guardian/SK_Gruntling_Guardian.SK_Gruntling_Guardian"));

	int SkeletalMeshNumber = FMath::RandRange(1, 3);

	switch (SkeletalMeshNumber)
	{
	case 1:
		if (SK_CARDBOARD1.Succeeded())
			GetMesh()->SetSkeletalMesh(SK_CARDBOARD1.Object);
		break;
	case 2:
		if (SK_CARDBOARD2.Succeeded())
			GetMesh()->SetSkeletalMesh(SK_CARDBOARD2.Object);
		break;
	case 3:
		if (SK_CARDBOARD3.Succeeded())
			GetMesh()->SetSkeletalMesh(SK_CARDBOARD3.Object);
		break;
	default:
		break;
	}

	// �ִϸ��̼� �������Ʈ ����ϱ�
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// �ִϸ��̼� �������Ʈ�� Ŭ����(�ִ� �ν��Ͻ�)�̱� ������ ��� ���� "_C"�� �߰��� �ٿ���� �Ѵ�.
	static ConstructorHelpers::FClassFinder<UAnimInstance> MONSTER_ANIM(TEXT("/Game/Enemy/Enemy_Gruntling_Guardian/Gruntling_Guardian_BluePrint.Gruntling_Guardian_BluePrint_C"));
	if (MONSTER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MONSTER_ANIM.Class);
	}
	
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
}

// Called when the game starts or when spawned
void AFPMonster::BeginPlay()
{
	Super::BeginPlay();
	

	auto FPGameMode = Cast<AFPGameMode>(GetWorld()->GetAuthGameMode());
	FPCHECK(nullptr != FPGameMode);
	int32 TargetLevel = FMath::CeilToInt(((float)FPGameMode->GetScore() * 0.8f));
	int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
	FPLOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
	CharacterStat->SetNewLevel(FinalLevel);

	auto CharacterWidget = Cast<UFPCharacterWidget>(HPBarWidget->GetUserWidgetObject());
	FPCHECK(nullptr != CharacterWidget);
	CharacterWidget->BindCharacterStat(CharacterStat);

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);

	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	//FPAIController->RUNAI();
}

// Called every frame
void AFPMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


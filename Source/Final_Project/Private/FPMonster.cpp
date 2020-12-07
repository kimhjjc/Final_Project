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

	// 메쉬를 코드로 불러서 설정해주기
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

	// 애니메이션 블루프린트 사용하기
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// 애니메이션 블루프린트는 클래스(애님 인스턴스)이기 때문에 경로 끝에 "_C"를 추가로 붙여줘야 한다.
	static ConstructorHelpers::FClassFinder<UAnimInstance> MONSTER_ANIM(TEXT("/Game/Enemy/Enemy_Gruntling_Guardian/Gruntling_Guardian_BluePrint.Gruntling_Guardian_BluePrint_C"));
	if (MONSTER_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MONSTER_ANIM.Class);
	}
	
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


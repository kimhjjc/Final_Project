// Fill out your copyright notice in the Description page of Project Settings.




#include "FPPawn.h"

// Sets default values
AFPPawn::AFPPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MESH"));
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MOVEMENT"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINTARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	RootComponent = Capsule;
	Mesh->SetupAttachment(Capsule);
	SpringArm->SetupAttachment(Capsule);
	Camera->SetupAttachment(SpringArm);

	// ĸ�� �ݶ��̴� ����� �κ�
	Capsule->SetCapsuleHalfHeight(88.0f);
	Capsule->SetCapsuleRadius(34.0f);
	// �޽� �߽��� �� ����(Ʈ������ ��ġ)
	Mesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	// �������� �Ÿ� �� �����̼� ���� (ī�޶�)
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	// �޽��� �ڵ�� �ҷ��� �������ֱ�
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Warrior.SK_CharM_Warrior"));
	if (SK_CARDBOARD.Succeeded())
	{
		Mesh->SetSkeletalMesh(SK_CARDBOARD.Object);
	}


	// �ִϸ��̼� �������Ʈ ����ϱ�
	Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// �ִϸ��̼� �������Ʈ�� Ŭ����(�ִ� �ν��Ͻ�)�̱� ������ ��� ���� "_C"�� �߰��� �ٿ���� �Ѵ�.
	static ConstructorHelpers::FClassFinder<UAnimInstance> WARRIOR_ANIM(TEXT("/Game/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		Mesh->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

}

// Called when the game starts or when spawned
void AFPPawn::BeginPlay()
{
	Super::BeginPlay();
	
	/*
	// �ڵ�� �ִϸ��̼� �����ϱ�
	Mesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	UAnimationAsset* AnimAsset = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/Animations/WarriorRun.WarriorRun"));
	if (AnimAsset != nullptr)
	{
		Mesh->PlayAnimation(AnimAsset, true);
	}
	*/
}

// Called every frame
void AFPPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Axis ���� Ȱ���Ͽ� �̵�Ű �����
	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AFPPawn::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AFPPawn::LeftRight);

}

void AFPPawn::UpDown(float NewAxisValue)
{
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void AFPPawn::LeftRight(float NewAxisValue)
{
	AddMovementInput(GetActorRightVector(), NewAxisValue);
}


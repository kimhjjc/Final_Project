// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/FPItemBox.h"
#include "Item/FPWeapon.h"
#include "Characters/Player/FPCharacter.h"

// Sets default values
AFPItemBox::AFPItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	
	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	// FPWeapon�� Ŭ����(����)�� �ҷ��´�.
	WeaponItemClass = AFPWeapon::StaticClass();

}

// Called when the game starts or when spawned
void AFPItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// BoxComponent����  OnComponentBeginOverlap�̶�� ��Ƽĳ���� ��������Ʈ�� �ִ�. �̸� �̿��Ͽ� �ڽ�������Ʈ�� ��ħ �ݸ��� ���� ���� �� ��������Ʈ �߻�.
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AFPItemBox::OnCharacterOverlap);
}

void AFPItemBox::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	FPLOG_S(Warning);

	auto FPCharacter = Cast<AFPCharacter>(OtherActor);
	FPCHECK(nullptr != FPCharacter);

	if (nullptr != FPCharacter && nullptr != WeaponItemClass)
	{
		if (FPCharacter->CanSetWeapon())
		{
			auto NewWeapon = GetWorld()->SpawnActor<AFPWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			FPCharacter->Setweapon(NewWeapon);
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);	// �� ���ӿ��� ������ �ʰ� �����.
			SetActorEnableCollision(false);
			// ��ƼŬ ������Ʈ�� OnSystemFinished ��������Ʈ��, ��ƼŬ�� ����� ������ �� �� �ȿ� ��Ƽĳ���õ� �Լ����� ȣ�����ִ� �����̴�.
			Effect->OnSystemFinished.AddDynamic(this, &AFPItemBox::OnEffectFinished);
		}
		else
		{
			FPLOG(Warning, TEXT("%s can't equip weapon currently."), *FPCharacter->GetName());
		}
	}
}

void AFPItemBox::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	Destroy();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "FPWeapon.h"

// Sets default values
AFPWeapon::AFPWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/AdvancedMagicFX12/Meshes/SK_ky_bigSword_anim.SK_ky_bigSword_anim"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SK_Blade_HeroSword11.SK_Blade_HeroSword11"));
	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);
	}

	/*static ConstructorHelpers::FObjectFinder<UAnimSequence> ET_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SK_Blade_HeroSword_01_Skeleton_Sequence.SK_Blade_HeroSword_01_Skeleton_Sequence"));
	if (ET_WEAPON.Succeeded())
	{
		Effect_Trail = ET_WEAPON.Object;
		Effect_Trail->SetSkeleton(SK_WEAPON.Object->Skeleton);
	}*/
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	AttackRange = 150.0f;
	AttackDamageMin = -2.5f;
	AttackDamageMax = 10.0f;
	AttackModifierMin = 0.85f;
	AttackModifierMax = 1.25f;


}

float AFPWeapon::GetAttackRange() const
{
	return AttackRange;
}

float AFPWeapon::GetAttackDamage() const
{
	return AttackDamage;
}

float AFPWeapon::GetAttackModifier() const
{
	return AttackModifier;
}


// Called when the game starts or when spawned
void AFPWeapon::BeginPlay()
{
	Super::BeginPlay();

	UAnimationAsset* Effect_Trail = LoadObject<UAnimationAsset>(nullptr, TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_HeroSword11/SK_Blade_HeroSword_01_Skeleton_Sequence.SK_Blade_HeroSword_01_Skeleton_Sequence"));

	Weapon->SetAnimationMode(EAnimationMode::AnimationSingleNode);
	//Weapon->SetAnimation(Effect_Trail);
	if (Effect_Trail != nullptr)
	{
		Weapon->PlayAnimation(Effect_Trail, true);
	}

	AttackDamage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	AttackModifier = FMath::RandRange(AttackModifierMin, AttackModifierMax);

	FPLOG(Warning, TEXT("Weapon Damage : %f, Modifier : %f"), AttackDamage, AttackModifier);
}

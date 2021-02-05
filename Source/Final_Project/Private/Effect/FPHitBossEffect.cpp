// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/FPHitBossEffect.h"

// Sets default values
AFPHitBossEffect::AFPHitBossEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Effect;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FIRE_EFFECT(TEXT("/Game/AdvancedMagicFX12/particles/P_ky_hit_dark.P_ky_hit_dark"));
	if (FIRE_EFFECT.Succeeded())
	{
		Effect->SetTemplate(FIRE_EFFECT.Object);
	}

	SetActorScale3D(FVector(0.7f, 0.7f, 0.7f));
}

// Called when the game starts or when spawned
void AFPHitBossEffect::BeginPlay()
{
	Super::BeginPlay();


	Effect->OnSystemFinished.AddDynamic(this, &AFPHitBossEffect::OnEffectFinished);
}

// Called every frame
void AFPHitBossEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPHitBossEffect::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	Destroy();
}

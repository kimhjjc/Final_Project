// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/FPHitPunchEffect.h"

// Sets default values
AFPHitPunchEffect::AFPHitPunchEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Effect;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FIRE_EFFECT(TEXT("/Game/AdvancedMagicFX12/particles/P_ky_hit_shine.P_ky_hit_shine"));
	if (FIRE_EFFECT.Succeeded())
	{
		Effect->SetTemplate(FIRE_EFFECT.Object);
	}

	SetActorScale3D(FVector(0.3f, 0.3f, 0.3f));
}

// Called when the game starts or when spawned
void AFPHitPunchEffect::BeginPlay()
{
	Super::BeginPlay();


	Effect->OnSystemFinished.AddDynamic(this, &AFPHitPunchEffect::OnEffectFinished);
}

// Called every frame
void AFPHitPunchEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPHitPunchEffect::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	Destroy();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "FPHitEffect.h"

// Sets default values
AFPHitEffect::AFPHitEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Effect;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> FIRE_EFFECT(TEXT("/Game/AdvancedMagicFX12/particles/P_ky_hit_fire.P_ky_hit_fire"));
	if (FIRE_EFFECT.Succeeded())
	{
		Effect->SetTemplate(FIRE_EFFECT.Object);
	}

	SetActorScale3D(FVector(0.7f, 0.7f, 0.7f));
}

// Called when the game starts or when spawned
void AFPHitEffect::BeginPlay()
{
	Super::BeginPlay();


	Effect->OnSystemFinished.AddDynamic(this, &AFPHitEffect::OnEffectFinished);
}

// Called every frame
void AFPHitEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPHitEffect::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	Destroy();
}

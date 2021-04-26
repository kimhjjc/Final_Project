// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/FPLevelUpEffect.h"

// Sets default values
AFPLevelUpEffect::AFPLevelUpEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Effect;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> LevelUp_EFFECT(TEXT("/Game/AdvancedMagicFX12/particles/P_ky_aura_thunder.P_ky_aura_thunder"));
	if (LevelUp_EFFECT.Succeeded())
	{
		Effect->SetTemplate(LevelUp_EFFECT.Object);
	}
}

// Called when the game starts or when spawned
void AFPLevelUpEffect::BeginPlay()
{
	Super::BeginPlay();
	
	EffectScale = 1.0;
	DisappearStart = false;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
		DisappearStart = true;
		}), 1.5f, false);
}

// Called every frame
void AFPLevelUpEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

	if (DisappearStart)
	{
		EffectScale -= 0.1f;
		if (EffectScale <= 0.0f)
			Destroy();

		SetActorScale3D(FVector(EffectScale, EffectScale, EffectScale));
	}
}
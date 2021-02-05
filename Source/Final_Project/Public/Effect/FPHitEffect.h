// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Actor.h"
#include "FPHitEffect.generated.h"

UCLASS()
class FINAL_PROJECT_API AFPHitEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPHitEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;

private:
	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);

};

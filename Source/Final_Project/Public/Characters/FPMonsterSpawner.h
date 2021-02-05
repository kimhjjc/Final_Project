// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Actor.h"
#include "FPMonsterSpawner.generated.h"

UCLASS()
class FINAL_PROJECT_API AFPMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPMonsterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = Collision)
	UCapsuleComponent* Capsule;

private:
	void SpawnMonster();

private:
	FTimerHandle SpawnTimerHandle = { };

	//TArray<class AFPMonster*> SpawnedMonsters;
	//class AFPMonster* SpawnedMonsters[3];
	class AFPMonster* SpawnedMonster_1;
	class AFPMonster* SpawnedMonster_2;
	class AFPMonster* SpawnedMonster_3;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	int32 MaxSpawnAble;
};

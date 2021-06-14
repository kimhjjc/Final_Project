// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FPMonsterSpawner.h"
#include "Characters/Enemy/FPMonster.h"

// Sets default values
AFPMonsterSpawner::AFPMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CAPSULE"));

	RootComponent = Capsule;

	// 캡슐 콜라이더 만드는 부분
	//Capsule->SetCapsuleHalfHeight(0.0f);
	Capsule->SetCapsuleRadius(34.0f);


	MaxSpawnAble = 3;

	//SpawnedMonsters.Init(nullptr, MaxSpawnAble);
}

// Called when the game starts or when spawned
void AFPMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
		SpawnMonster();
		}), 7.0f, true);

}

// Called every frame
void AFPMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPMonsterSpawner::SpawnMonster()
{
	//for (int i = 0; i < MaxSpawnAble; i++)
	//{
	//	if (SpawnedMonsters[i] == nullptr)
	//	{
	//		auto SpawnLocation = GetActorLocation() + FVector(rand() % 360, rand() % 360, 0.0f);
	//		auto SpawnedMonster = GetWorld()->SpawnActor<AFPMonster>(AFPMonster::StaticClass(), SpawnLocation, FRotator(0.0f, rand() % 360, 0.0f));
	//		SpawnedMonsters[i] = SpawnedMonster;

	//	}
	//	
	//}

	if (SpawnedMonster_1 == nullptr)
	{
		auto SpawnLocation = GetActorLocation() + FVector(-180.0f, 0, 0.0f);
		auto SpawnedMonster = GetWorld()->SpawnActor<AFPMonster>(AFPMonster::StaticClass(), SpawnLocation, FRotator(0.0f, rand() % 360, 0.0f));
		SpawnedMonster_1 = SpawnedMonster;

		SpawnedMonster_1->OnHPIsZero.AddLambda([this]() -> void {
			SpawnedMonster_1 = nullptr;
			});
	}
	
	if (SpawnedMonster_2 == nullptr)
	{
		auto SpawnLocation = GetActorLocation() + FVector(180.0f, 0, 0.0f);
		auto SpawnedMonster = GetWorld()->SpawnActor<AFPMonster>(AFPMonster::StaticClass(), SpawnLocation, FRotator(0.0f, rand() % 360, 0.0f));
		SpawnedMonster_2 = SpawnedMonster;

		SpawnedMonster_2->OnHPIsZero.AddLambda([this]() -> void {
			SpawnedMonster_2 = nullptr;
			});
	}

	if (SpawnedMonster_3 == nullptr)
	{
		auto SpawnLocation = GetActorLocation() + FVector(0, 180.0f, 0.0f);
		auto SpawnedMonster = GetWorld()->SpawnActor<AFPMonster>(AFPMonster::StaticClass(), SpawnLocation, FRotator(0.0f, rand() % 360, 0.0f));
		SpawnedMonster_3 = SpawnedMonster;

		SpawnedMonster_3->OnHPIsZero.AddLambda([this]() -> void {
			SpawnedMonster_3 = nullptr;
			});
	}
	//auto SpawnLocation = GetActorLocation() + FVector(rand() % 360, rand() % 360, 0.0f);
	//auto SpawnedMonster = GetWorld()->SpawnActor<AFPMonster>(AFPMonster::StaticClass(), SpawnLocation, FRotator(0.0f, rand() % 360, 0.0f));

}


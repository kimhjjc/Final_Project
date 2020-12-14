// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacterStatComponent.h"
#include "FPGameInstance.h"

// Sets default values for this component's properties
UFPCharacterStatComponent::UFPCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;	// InitializeComponent �Լ��� ����Ϸ��� �� bool���� true�� �������� �ٲ��־�� �Ѵ�.
	// ...

	Level = 1;
}


// Called when the game starts
void UFPCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UFPCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);

	OnHPChanged.AddLambda([this]() -> void {
		if (IsRecoveringHP)
		{
			IsRecoveringHP = false;
			OnRecoverHP.Broadcast();
		}
		});
	OnHPChanged.AddLambda([this]() -> void {
		if (IsDamagingHP)
		{
			IsDamagingHP = false;
			OnDamagedHP.Broadcast();
		}
		});
}

void UFPCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	auto FPGameInstance = Cast<UFPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	FPCHECK(nullptr != FPGameInstance);
	CurrentStatData = FPGameInstance->GetFPCharacterData(NewLevel);
	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		//CurrentHP = CurrentStatData->MaxHP;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		FPLOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void UFPCharacterStatComponent::SetHeal(float NewRecoverHP)
{
	FPCHECK(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP + NewRecoverHP * (CurrentStatData->MaxHP / 100.0f), 0.0f, CurrentStatData->MaxHP));
}

void UFPCharacterStatComponent::SetDamage(float NewDamage)
{
	FPCHECK(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UFPCharacterStatComponent::SetHP(float NewHP)
{
	if (CurrentHP < NewHP)
		IsRecoveringHP = true;
	else if (CurrentHP > NewHP)
		IsDamagingHP = true;

	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	// float�� ���� 0�� ���� ���� �̼��� ���� ���� ���� �ִ����� ���� �Ǵ��ϴ� ���� ����. �׷��� �𸮾󿡼��� KINDA_SMALL_NUMBER��� ��ũ�θ� ���� �׷��� �Ǵ��� �����ϰ� ���ش�.
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float UFPCharacterStatComponent::GetAttack()
{
	FPCHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

float UFPCharacterStatComponent::GetHPRatio()
{
	FPCHECK(nullptr != CurrentStatData, 0.0f);
	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

int32 UFPCharacterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}

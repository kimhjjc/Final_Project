// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Components/ActorComponent.h"
#include "FPCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FINAL_PROJECT_API UFPCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFPCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;	// PostInitializeComponents�� �����ϴ� ������Ʈ��, �� �Լ��� ������ PostInitializeComponents�Լ��� ȣ��Ǳ� �ٷ� ���� ȣ��ȴ�.

public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	float GetAttack();
	float GetHPRatio();
	int32 GetDropExp() const;

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

private:
	struct FFPCharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	// �𸮾� ������Ʈ���� ����ȭ ����� �־ UPROPERTY �Ӽ��� ����, �ε��� �����ѵ�, �� �� TransientŰ���带 �߰��ϸ� �ش� �Ӽ��� ����ȭ���� ���ܽ�ų �� �ִ�.
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
		
};

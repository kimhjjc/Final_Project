// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Actor.h"
#include "FPItemBox.generated.h"

UCLASS()
class FINAL_PROJECT_API AFPItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	UPROPERTY(VisibleAnywhere, Category = Box)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;

	// �𸮾󿡼��� Ư�� Ŭ������ ��ӹ��� Ŭ������� ����� �����ϵ��� TSubclassof��� Ű���带 �����Ѵ�. �̸� ����ϸ� ��Ͽ��� ������ ���ڿ� �̸� ������ Ŭ���� ��ϸ� �� �� �ִ�. 
	// UClass()�� �����͸� ����ص� ������ �׷� ���� ������Ʈ�� ����ϴ� ��� �𸮾� ������Ʈ�� ������ ���̰� �ȴ�.
	UPROPERTY(EditInstanceOnly, Category = Box)
	TSubclassOf<class AFPWeapon> WeaponItemClass;

private:
	// ���̳��� �Լ��� UFUNCTION()�� ���� ������ ������� ����.
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);
};

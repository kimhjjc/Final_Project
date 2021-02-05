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

	// 언리얼에서는 특정 클래스와 상속받은 클래스들로 목록을 한정하도록 TSubclassof라는 키워드를 제공한다. 이를 사용하면 목록에서 아이템 상자와 이를 선언한 클래스 목록만 볼 수 있다. 
	// UClass()의 포인터를 사용해도 되지만 그럼 현재 프로젝트에 사용하는 모든 언리얼 오브젝트의 선언이 보이게 된다.
	UPROPERTY(EditInstanceOnly, Category = Box)
	TSubclassOf<class AFPWeapon> WeaponItemClass;

private:
	// 다이나믹 함수에 UFUNCTION()을 쓰지 않으면 실행되지 않음.
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEffectFinished(class UParticleSystemComponent* PSystem);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "EngineMinimal.h"
#include "Final_Project.h"		// ����� �α׸� ����ϱ����� Final_Project.h���� EngineMinimal.h�� ��� include���ְ� ����
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

/*
1. �𸮾� ������Ʈ�� �ݵ�� �ٴ� ���λ簡 �ΰ� �ٴ´�. A�� U�� A�� ����, U�� ���Ͱ� �ƴ� Ŭ������ ����Ѵ�.
2. Ŭ���� �̸� �տ� (����_API �� �־�� �ٸ� ��ü���� ������ �� �ִ�.
3. UPROPERTY()�� ������ ��ü�� �ڵ����� �����ϰ� ������ִ� ��ũ�� �Լ��̴�. 
4. UPROPERTY()�� ��ȣ���� Ư�� Ű���尡 �� �� �ִ�. (Ű���带 �Է����� ������ �ش� ������Ʈ ���� �Ұ���)
5. UPROPERTY ���� �� �� ��� ������ 0�� �ʱ� ���� ����޴´�.
6. �𸮾��� �����ϴ� ��ǥ �� ���� : ����Ʈ uint8 / ���� int32 / �Ǽ� float / ���ڿ� FString, FName / ����ü FVector, FRotator, FTransform
7. �ֿ� �̺�Ʈ �Լ��� ����Ǵ� ������ PostInitiallizeComponents() -> BeginPlay() -> EndPlay() �����̴�.
8. Tick �Լ��� ���ڰ� DeltaTime�� GetWorld()->GetDeltaSeconds()�� ����.
	- ���� ���� �� ������� ����� �ð� : GetWorld()->GetTimeSeconds()
	- ����ڰ� ������ ������ �ð��� ������ ��� �ð� : GetWorld()->GetUnpausedTimeSeconds()
	- ���ǿ����� ��� �ð� : GetWorld()->GetRealTimeSeconds()
	- ���ǿ����� ������ �ð��� ������ ��� �ð� : GetWorld->GetAudioTimeSeconds()
9. movement���� FloatingPawnMovement, RotatingMovement, InterpMovement, ProjectileMovement�� �ִ�.
!!!. Unity�� ��ũ��Ʈ ����� �ٷ� �ݿ��� ������ �𸮾��� ������ ��ư�� �ٽ� ���������. (�ð� �����ɸ�)
*/


UCLASS()
class FINAL_PROJECT_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// CoreMinimal.h >> EngineMinimal.h �� �ٲپ� �־�� UStaticMeshComponent�� ������ ����.
	UPROPERTY(VisibleAnywhere)		// VisibleAnywhere = ������Ʈ �Ϻ� ���� ���� (�� ���� ��ü ���� ���� �Ұ�)
	UStaticMeshComponent * Body;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * Water;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent *Light;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent *Splash;

	// �̰� ����Ϸ��� GameFramework/RotatingMovementComponent ��������� �߰��ؾ���
	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

	UPROPERTY(EditAnywhere, Category = ID);	// EditAnywhere = �Ӽ��� ������ ���� ���� (������ �ַ� ���), Category = ID -> ������ ������Ʈ ������ ID ī�װ����� ���� (ī�װ��� ������ ����)
	int32 ID;

private:
	UPROPERTY(EditAnywhere, Category=Stat, Meta = (AllowPrivateAccess = true))	// Meta = (AllowPrivateAccess = true)�� ����ϸ� private������ �����Ϳ����� ������ �����ϰ�, ���� �����͸� �����Ͽ� ĸ��ȭ ���� ����������.
	float RotateSpeed;

};

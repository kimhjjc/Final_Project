// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "EngineMinimal.h"
#include "Final_Project.h"		// 디버깅 로그를 사용하기위해 Final_Project.h에서 EngineMinimal.h를 대신 include해주고 선언
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Actor.h"
#include "Fountain.generated.h"

/*
1. 언리얼 오브젝트는 반드시 붙는 접두사가 두개 붙는다. A와 U로 A는 액터, U는 액터가 아닌 클래스에 사용한다.
2. 클래스 이름 앞에 (모듈명_API 가 있어야 다른 객체에서 접근할 수 있다.
3. UPROPERTY()는 선언한 객체를 자동으로 관리하게 만들어주는 매크로 함수이다. 
4. UPROPERTY()의 괄호에는 특수 키워드가 들어갈 수 있다. (키워드를 입력하지 않으면 해당 컴포넌트 수정 불가능)
5. UPROPERTY 선언 시 그 멤버 변수는 0의 초기 값을 보장받는다.
6. 언리얼이 제공하는 대표 값 유형 : 바이트 uint8 / 정수 int32 / 실수 float / 문자열 FString, FName / 구조체 FVector, FRotator, FTransform
7. 주요 이벤트 함수가 실행되는 순서는 PostInitiallizeComponents() -> BeginPlay() -> EndPlay() 순서이다.
8. Tick 함수의 인자값 DeltaTime은 GetWorld()->GetDeltaSeconds()와 같다.
	- 게임 실행 후 현재까지 경과된 시간 : GetWorld()->GetTimeSeconds()
	- 사용자가 게임을 중지한 시간을 제외한 경과 시간 : GetWorld()->GetUnpausedTimeSeconds()
	- 현실에서의 경과 시간 : GetWorld()->GetRealTimeSeconds()
	- 현실에서의 중지한 시간을 제외한 경과 시간 : GetWorld->GetAudioTimeSeconds()
9. movement에는 FloatingPawnMovement, RotatingMovement, InterpMovement, ProjectileMovement가 있다.
!!!. Unity는 스크립트 저장시 바로 반영이 되지만 언리얼은 컴파일 버튼을 다시 눌러줘야함. (시간 오래걸림)
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

	// CoreMinimal.h >> EngineMinimal.h 로 바꾸어 주어야 UStaticMeshComponent를 참조가 가능.
	UPROPERTY(VisibleAnywhere)		// VisibleAnywhere = 컴포넌트 일부 수정 가능 (값 유형 객체 유형 수정 불가)
	UStaticMeshComponent * Body;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent * Water;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent *Light;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent *Splash;

	// 이걸 사용하려면 GameFramework/RotatingMovementComponent 헤더파일을 추가해야함
	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

	UPROPERTY(EditAnywhere, Category = ID);	// EditAnywhere = 속성의 데이터 편집 가능 (변수에 주로 사용), Category = ID -> 변수를 컴포넌트 섹션의 ID 카테고리에서 관리 (카테고리가 없으면 생성)
	int32 ID;

private:
	UPROPERTY(EditAnywhere, Category=Stat, Meta = (AllowPrivateAccess = true))	// Meta = (AllowPrivateAccess = true)를 사용하면 private이지만 에디터에서는 편집이 가능하고, 변수 데이터를 은닉하여 캡슐화 또한 가능해진다.
	float RotateSpeed;

};

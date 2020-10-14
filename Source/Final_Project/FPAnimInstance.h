// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Animation/AnimInstance.h"
#include "FPAnimInstance.generated.h"

/*
1. 애님 인스턴스는 애님 그래프가 참조할 데이터를 제공하는 클래스로 블루프린트와 C++로 제작이 가능하고, 애니 그래프는 블루프린트로만 제작이 가능하다.
2. 현재 움직임을 파악하기 위해 폰의 무브먼트 컴포넌트가 제공하는 함수는 다음과 같다.
	1) IsFalling() : 공중에 있는가		3) IsCrouching() : 쭈그리고 앉아있는가
	2) IsSwimming() : 수영중인가			4) IsMoveOnGround() : 땅 위에서 이동 중인가
 */


UCLASS()
class FINAL_PROJECT_API UFPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFPAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	// BlueprintReadOnly를 하면 애님 그래프에서 변수를 Get은 가능하지만 Set은 불가능함.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
};

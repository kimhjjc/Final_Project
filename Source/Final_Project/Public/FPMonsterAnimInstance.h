// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Animation/AnimInstance.h"
#include "FPMonsterAnimInstance.generated.h"

// Fill out your copyright notice in the Description page of Project Settings.
/*
1. 애님 인스턴스는 애님 그래프가 참조할 데이터를 제공하는 클래스로 블루프린트와 C++로 제작이 가능하고, 애니 그래프는 블루프린트로만 제작이 가능하다.
2. 현재 움직임을 파악하기 위해 폰의 무브먼트 컴포넌트가 제공하는 함수는 다음과 같다.
	1) IsFalling() : 공중에 있는가		3) IsCrouching() : 쭈그리고 앉아있는가
	2) IsSwimming() : 수영중인가			4) IsMoveOnGround() : 땅 위에서 이동 중인가
 */

 // 이것은 멀티캐스트 델리게이트로, 델리게이트에 등록된 모든 함수를 호출하는 시스템이다. 멀티캐스트 델리게이트 명령은 (Delegate이름).Broadcast이다.
 // 사용시 장점 : 델리게이트에 연결된 함수만 호출하면 되므로 다른 클래스와 연결되지 않는 의존성 없는 설계를 할 수 있다.
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

UCLASS()
class FINAL_PROJECT_API UFPMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFPMonsterAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();

public:
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private:
	//UFUCTION()은 블루프린트의 함수를 C++에서 연동하여 사용할때 함수 앞에 선언해주어야 한다.
	UFUNCTION()
	void AnimNotify_AttackHitCheck();		// 몽타주에서 노티파이가 호출되려면 AnimNotify_노티파이명 으로 선언해야 이름의 멤버 함수를 찾아서 호출이 된다. 

private:
	// BlueprintReadOnly를 하면 애님 그래프에서 변수를 Get은 가능하지만 Set은 불가능함.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

};

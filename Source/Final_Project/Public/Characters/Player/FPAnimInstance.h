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

// 이것은 멀티캐스트 델리게이트로, 델리게이트에 등록된 모든 함수를 호출하는 시스템이다. 멀티캐스트 델리게이트 명령은 (Delegate이름).Broadcast이다.
// 사용시 장점 : 델리게이트에 연결된 함수만 호출하면 되므로 다른 클래스와 연결되지 않는 의존성 없는 설계를 할 수 있다.
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMotionBasedMovementDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMotionBasedMovementFinishDelegate);
DECLARE_MULTICAST_DELEGATE(FOnFinishMontageDelegate);

DECLARE_MULTICAST_DELEGATE(FOnRest_LoopingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnRest_FinishDelegate);

DECLARE_MULTICAST_DELEGATE(FOnFire_ShootingDelegate);

UCLASS()
class FINAL_PROJECT_API UFPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFPAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

	void PlayAttack_RMBMontage();

	void PlayRest_Montage();
	void JumpToRest_End_MontageSection();

	void PlayRoll_Montage();
	void PlayFireShot_Montage();
public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnMotionBasedMovementDelegate OnMotionBasedMovement;
	FOnMotionBasedMovementFinishDelegate OnMotionBasedMovementFinish;
	FOnFinishMontageDelegate OnFinishMontage;

	FOnRest_LoopingDelegate OnRest_Looping;
	FOnRest_FinishDelegate OnRest_Finish;

	FOnFire_ShootingDelegate OnFire_Shooting;
	void SetDeadAnim() { IsDead = true; }

private:
	//UFUCTION()은 블루프린트의 함수를 C++에서 연동하여 사용할때 함수 앞에 선언해주어야 한다.
	UFUNCTION()
	void AnimNotify_AttackHitCheck();		// 몽타주에서 노티파이가 호출되려면 AnimNotify_노티파이명 으로 선언해야 이름의 멤버 함수를 찾아서 호출이 된다. 

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_MotionBasedMovement();

	UFUNCTION()
	void AnimNotify_MotionBasedMovementFinish();

	UFUNCTION()
	void AnimNotify_FinishMontage();

	UFUNCTION()
	void AnimNotify_Rest_Looping();

	UFUNCTION()
	void AnimNotify_Rest_Finish();

	UFUNCTION()
	void AnimNotify_Fire_Shooting();

	FName GetAttackMontageSectionName(int32 Section);

private:
	// BlueprintReadOnly를 하면 애님 그래프에서 변수를 Get은 가능하지만 Set은 불가능함.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Attack_RMBMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Rest_Montage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Roll_Montage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = true))
	UAnimMontage* FireShot_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

};

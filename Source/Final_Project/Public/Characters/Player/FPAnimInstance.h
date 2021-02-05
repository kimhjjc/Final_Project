// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Animation/AnimInstance.h"
#include "FPAnimInstance.generated.h"

/*
1. �ִ� �ν��Ͻ��� �ִ� �׷����� ������ �����͸� �����ϴ� Ŭ������ �������Ʈ�� C++�� ������ �����ϰ�, �ִ� �׷����� �������Ʈ�θ� ������ �����ϴ�.
2. ���� �������� �ľ��ϱ� ���� ���� �����Ʈ ������Ʈ�� �����ϴ� �Լ��� ������ ����.
	1) IsFalling() : ���߿� �ִ°�		3) IsCrouching() : �ޱ׸��� �ɾ��ִ°�
	2) IsSwimming() : �������ΰ�			4) IsMoveOnGround() : �� ������ �̵� ���ΰ�
 */

// �̰��� ��Ƽĳ��Ʈ ��������Ʈ��, ��������Ʈ�� ��ϵ� ��� �Լ��� ȣ���ϴ� �ý����̴�. ��Ƽĳ��Ʈ ��������Ʈ ����� (Delegate�̸�).Broadcast�̴�.
// ���� ���� : ��������Ʈ�� ����� �Լ��� ȣ���ϸ� �ǹǷ� �ٸ� Ŭ������ ������� �ʴ� ������ ���� ���踦 �� �� �ִ�.
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMotionBasedMovementDelegate);
DECLARE_MULTICAST_DELEGATE(FOnMotionBasedMovementFinishDelegate);
DECLARE_MULTICAST_DELEGATE(FOnFinishMontageDelegate);

DECLARE_MULTICAST_DELEGATE(FOnRest_LoopingDelegate);
DECLARE_MULTICAST_DELEGATE(FOnRest_FinishDelegate);

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

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnMotionBasedMovementDelegate OnMotionBasedMovement;
	FOnMotionBasedMovementFinishDelegate OnMotionBasedMovementFinish;
	FOnFinishMontageDelegate OnFinishMontage;

	FOnRest_LoopingDelegate OnRest_Looping;
	FOnRest_FinishDelegate OnRest_Finish;
	void SetDeadAnim() { IsDead = true; }

private:
	//UFUCTION()�� �������Ʈ�� �Լ��� C++���� �����Ͽ� ����Ҷ� �Լ� �տ� �������־�� �Ѵ�.
	UFUNCTION()
	void AnimNotify_AttackHitCheck();		// ��Ÿ�ֿ��� ��Ƽ���̰� ȣ��Ƿ��� AnimNotify_��Ƽ���̸� ���� �����ؾ� �̸��� ��� �Լ��� ã�Ƽ� ȣ���� �ȴ�. 

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

	FName GetAttackMontageSectionName(int32 Section);

private:
	// BlueprintReadOnly�� �ϸ� �ִ� �׷������� ������ Get�� ���������� Set�� �Ұ�����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Attack_RMBMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Rest_Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

};

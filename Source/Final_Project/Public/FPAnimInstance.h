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

UCLASS()
class FINAL_PROJECT_API UFPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFPAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	void SetDeadAnim() { IsDead = true; }

private:
	//UFUCTION()�� �������Ʈ�� �Լ��� C++���� �����Ͽ� ����Ҷ� �Լ� �տ� �������־�� �Ѵ�.
	UFUNCTION()
	void AnimNotify_AttackHitCheck();		// ��Ÿ�ֿ��� ��Ƽ���̰� ȣ��Ƿ��� AnimNotify_��Ƽ���̸� ���� �����ؾ� �̸��� ��� �Լ��� ã�Ƽ� ȣ���� �ȴ�. 

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);

private:
	// BlueprintReadOnly�� �ϸ� �ִ� �׷������� ������ Get�� ���������� Set�� �Ұ�����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

};

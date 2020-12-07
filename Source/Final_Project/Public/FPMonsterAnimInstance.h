// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Animation/AnimInstance.h"
#include "FPMonsterAnimInstance.generated.h"

// Fill out your copyright notice in the Description page of Project Settings.
/*
1. �ִ� �ν��Ͻ��� �ִ� �׷����� ������ �����͸� �����ϴ� Ŭ������ �������Ʈ�� C++�� ������ �����ϰ�, �ִ� �׷����� �������Ʈ�θ� ������ �����ϴ�.
2. ���� �������� �ľ��ϱ� ���� ���� �����Ʈ ������Ʈ�� �����ϴ� �Լ��� ������ ����.
	1) IsFalling() : ���߿� �ִ°�		3) IsCrouching() : �ޱ׸��� �ɾ��ִ°�
	2) IsSwimming() : �������ΰ�			4) IsMoveOnGround() : �� ������ �̵� ���ΰ�
 */

 // �̰��� ��Ƽĳ��Ʈ ��������Ʈ��, ��������Ʈ�� ��ϵ� ��� �Լ��� ȣ���ϴ� �ý����̴�. ��Ƽĳ��Ʈ ��������Ʈ ����� (Delegate�̸�).Broadcast�̴�.
 // ���� ���� : ��������Ʈ�� ����� �Լ��� ȣ���ϸ� �ǹǷ� �ٸ� Ŭ������ ������� �ʴ� ������ ���� ���踦 �� �� �ִ�.
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
	//UFUCTION()�� �������Ʈ�� �Լ��� C++���� �����Ͽ� ����Ҷ� �Լ� �տ� �������־�� �Ѵ�.
	UFUNCTION()
	void AnimNotify_AttackHitCheck();		// ��Ÿ�ֿ��� ��Ƽ���̰� ȣ��Ƿ��� AnimNotify_��Ƽ���̸� ���� �����ؾ� �̸��� ��� �Լ��� ã�Ƽ� ȣ���� �ȴ�. 

private:
	// BlueprintReadOnly�� �ϸ� �ִ� �׷������� ������ Get�� ���������� Set�� �Ұ�����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDead;

};

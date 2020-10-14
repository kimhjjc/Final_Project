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


UCLASS()
class FINAL_PROJECT_API UFPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFPAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	// BlueprintReadOnly�� �ϸ� �ִ� �׷������� ������ Get�� ���������� Set�� �Ұ�����.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsInAir;
};

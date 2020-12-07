// Fill out your copyright notice in the Description page of Project Settings.


#include "FPMonsterAnimInstance.h"

// Fill out your copyright notice in the Description page of Project Settings.

UFPMonsterAnimInstance::UFPMonsterAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Enemy/Enemy_Gruntling_Guardian/Mob_AttackMontage.Mob_AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

// ���� �ӷ� ���� ���ϱ� ���� ���
void UFPMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// TryGetPawnOwner() == ���� �ӷ� ��
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;


	//if (::IsValid(Pawn))
	if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}

void UFPMonsterAnimInstance::PlayAttackMontage()
{
	// Montage_IsPlaying �Լ��� ��������Ʈ�� �̿��Ͽ� ���۰� ���ᰡ �����ǹǷ� ������� �ʾƵ� �����ϴ�.
	//if (!Montage_IsPlaying(AttackMontage))
	FPCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);

}

void UFPMonsterAnimInstance::AnimNotify_AttackHitCheck()
{
	FPLOG_S(Warning);
	OnAttackHitCheck.Broadcast();	// OnAttackHitCheck��� ��Ƽĳ��Ʈ ��������Ʈ�� ��ϵ� ��� �Լ��� ȣ��.
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "FPMonsterAnimInstance.h"

// Fill out your copyright notice in the Description page of Project Settings.

UFPMonsterAnimInstance::UFPMonsterAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsDead = false;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> ENEMY_ATTACK_MONTAGE(TEXT("/Game/Enemy/Enemy_Gruntling_Guardian/Mob_AttackMontage_1.Mob_AttackMontage_1"));
	if (ENEMY_ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ENEMY_ATTACK_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SPAIDERBOSS_ATTACK_MONTAGE(TEXT("/Game/Enemy/Enemy_Great_Spider/SpiderBossAttackMontage.SpiderBossAttackMontage"));
	if (SPAIDERBOSS_ATTACK_MONTAGE.Succeeded())
	{
		BossAttackMontage = SPAIDERBOSS_ATTACK_MONTAGE.Object;
	}
}

// 폰의 속력 값을 구하기 위해 사용
void UFPMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// TryGetPawnOwner() == 폰의 속력 값
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
	// Montage_IsPlaying 함수는 델리게이트를 이용하여 시작과 종료가 감지되므로 사용하지 않아도 무방하다.
	//if (!Montage_IsPlaying(AttackMontage))
	FPCHECK(!IsDead);

	Montage_Play(AttackMontage, 1.0f);
}

void UFPMonsterAnimInstance::PlayBossAttackMontage()
{
	// Montage_IsPlaying 함수는 델리게이트를 이용하여 시작과 종료가 감지되므로 사용하지 않아도 무방하다.
	//if (!Montage_IsPlaying(AttackMontage))
	FPCHECK(!IsDead);

	Montage_Play(BossAttackMontage, 1.0f);
}

void UFPMonsterAnimInstance::AnimNotify_AttackHitCheck()
{
	FPLOG_S(Warning);
	OnAttackHitCheck.Broadcast();	// OnAttackHitCheck라는 멀티캐스트 델리게이트에 등록된 모든 함수를 호출.
}

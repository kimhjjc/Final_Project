// Fill out your copyright notice in the Description page of Project Settings.


#include "FPAnimInstance.h"

UFPAnimInstance::UFPAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
	
}

// 폰의 속력 값을 구하기 위해 사용
void UFPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// TryGetPawnOwner() == 폰의 속력 값
	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;


	//if (::IsValid(Pawn))
	if(!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UFPAnimInstance::PlayAttackMontage()
{
	// Montage_IsPlaying 함수는 델리게이트를 이용하여 시작과 종료가 감지되므로 사용하지 않아도 무방하다.
	//if (!Montage_IsPlaying(AttackMontage))
	FPCHECK(!IsDead);
		Montage_Play(AttackMontage, 1.0f);

}

void UFPAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	FPCHECK(!IsDead);
	FPCHECK(Montage_IsPlaying(AttackMontage));
	// 몽타주 섹션 점프 기본함수.
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UFPAnimInstance::AnimNotify_AttackHitCheck()
{
	FPLOG_S(Warning);
	OnAttackHitCheck.Broadcast();	// OnAttackHitCheck라는 멀티캐스트 델리게이트에 등록된 모든 함수를 호출.
}

void UFPAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UFPAnimInstance::AnimNotify_MotionBasedMovement()
{
	OnMotionBasedMovement.Broadcast();
}

void UFPAnimInstance::AnimNotify_MotionBasedMovementFinish()
{
	OnMotionBasedMovementFinish.Broadcast();
}

FName UFPAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	FPCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

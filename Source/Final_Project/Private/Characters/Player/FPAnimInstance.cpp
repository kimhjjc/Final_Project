// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/FPAnimInstance.h"

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
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_RMB_MONTAGE(TEXT("/Game/Animations/SK_Mannequin_Skeleton2.SK_Mannequin_Skeleton2"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		Attack_RMBMontage = ATTACK_RMB_MONTAGE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> REST_MONTAGE(TEXT("/Game/Animations/Rest_Montage.Rest_Montage"));
	if (REST_MONTAGE.Succeeded())
	{
		Rest_Montage = REST_MONTAGE.Object;
	}
}

// ���� �ӷ� ���� ���ϱ� ���� ���
void UFPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// TryGetPawnOwner() == ���� �ӷ� ��
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
	// Montage_IsPlaying �Լ��� ��������Ʈ�� �̿��Ͽ� ���۰� ���ᰡ �����ǹǷ� ������� �ʾƵ� �����ϴ�.
	//if (!Montage_IsPlaying(AttackMontage))
	FPCHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);

}

void UFPAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	FPCHECK(!IsDead);
	FPCHECK( (Montage_IsPlaying(AttackMontage) || Montage_IsPlaying(Attack_RMBMontage)) );
	// ��Ÿ�� ���� ���� �⺻�Լ�.
	Montage_Play(AttackMontage, 1.0f);
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UFPAnimInstance::PlayAttack_RMBMontage()
{
	FPCHECK(!IsDead);
	Montage_Play(Attack_RMBMontage, 1.0f);
}

void UFPAnimInstance::PlayRest_Montage()
{
	FPCHECK(!IsDead);
	Montage_Play(Rest_Montage, 1.0f);
}

void UFPAnimInstance::JumpToRest_End_MontageSection()
{
	Montage_JumpToSection(FName(TEXT("Rest_End")), Rest_Montage);
}

void UFPAnimInstance::AnimNotify_AttackHitCheck()
{
	FPLOG_S(Warning);
	OnAttackHitCheck.Broadcast();	// OnAttackHitCheck��� ��Ƽĳ��Ʈ ��������Ʈ�� ��ϵ� ��� �Լ��� ȣ��.
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

void UFPAnimInstance::AnimNotify_FinishMontage()
{
	OnFinishMontage.Broadcast();
}

void UFPAnimInstance::AnimNotify_Rest_Looping()
{
	OnRest_Looping.Broadcast();
}

void UFPAnimInstance::AnimNotify_Rest_Finish()
{
	OnRest_Finish.Broadcast();
}

FName UFPAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	FPCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}

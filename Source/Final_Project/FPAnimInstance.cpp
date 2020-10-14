// Fill out your copyright notice in the Description page of Project Settings.


#include "FPAnimInstance.h"

UFPAnimInstance::UFPAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
}

// 폰의 속력 값을 구하기 위해 사용
void UFPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// TryGetPawnOwner() == 폰의 속력 값
	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

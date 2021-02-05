// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTTask_TurnToTarget.h"
#include "Characters/FPAIController.h"
#include "Characters/Player/FPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");
}


EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;


	/*auto FPCharacter = Cast<AFPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == FPCharacter)
		return EBTNodeResult::Failed;*/


	auto Target = Cast<AFPCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AFPAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	LookVector.Z = 0.0f;		// 일단은 무조건 모두 Z = 0.0f에 있다는 가정하에 사용한다.
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	ControllingPawn->SetActorRotation(FMath::RInterpTo(ControllingPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));


	//FVector LookVector = Target->GetActorLocation() - FPCharacter->GetActorLocation();
	//LookVector.Z = 0.0f;		// 일단은 무조건 모두 Z = 0.0f에 있다는 가정하에 사용한다.
	//FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	//FPCharacter->SetActorRotation(FMath::RInterpTo(FPCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}

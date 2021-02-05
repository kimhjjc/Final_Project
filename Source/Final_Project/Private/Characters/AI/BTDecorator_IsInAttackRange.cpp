// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTDecorator_IsInAttackRange.h"
#include "Characters/FPAIController.h"
#include "Characters/Player/FPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	//auto ControllingPawn = Cast<AFPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;
	auto Target = Cast<AFPCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AFPAIController::TargetKey));
	if (nullptr == Target)
		return false;
	//bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetFinalAttackRange());
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 150.0);
	return bResult;
}

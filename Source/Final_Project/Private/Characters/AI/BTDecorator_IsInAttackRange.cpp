// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTDecorator_IsInAttackRange.h"
#include "Characters/FPAIController.h"
#include "Characters/Player/FPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Boss/FPSpaiderBoss.h"
#include "Characters/Boss/FPLastBoss.h"


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

	auto FPSpaiderBoss = Cast<AFPSpaiderBoss>(ControllingPawn);
	auto FPLastBoss = Cast<AFPLastBoss>(ControllingPawn);

	if (FPSpaiderBoss)
	{
		if (FPSpaiderBoss->GetIsAttacking())
			return true;
	}
	else if (FPLastBoss)
	{
		if (FPLastBoss->GetIsAttacking())
			return true;
	}

	auto Target = Cast<AFPCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AFPAIController::TargetKey));
	if (nullptr == Target)
		return false;
	//bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetFinalAttackRange());
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 150.0);
	return bResult;
}

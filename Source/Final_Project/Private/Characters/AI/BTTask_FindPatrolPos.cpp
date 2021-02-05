// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTTask_FindPatrolPos.h"
#include "Characters/FPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigationsystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	// 태스크의 이름을 다른 이름으로 표시하고 싶다면 이 NodeName속성을 다른 값으로 지정하면 된다.
	NodeName = TEXT("FindPatrolPos");
}


/*
	ExecuteTask함수는 반드시 넷 중 하나의 값을 반환해야한다.
	EBTNodeResult::Aborted	: 태스크 실행 중에 중단된다. 결과적으로 실패했다.
	EBTNodeResult::Failed	: 태스크를 수행했지만 실패했다.
	EBTNodeResult::Succeeded: 태스크를 성공적으로 수행했다.
	EBTNodeResult::Aborted	: 태스크를 계속 수행하고 있다. 태스크의 실행 결과는 향후 알려줄 예정이다.

	또한, 이 함수의 현재 사용 중인 시퀀스 콤포짓은 자신에 속한 태스크를 실패할 때까지 계속 실행하는 성질이 있다.
*/
EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//auto IsOnTarget = OwnerComp.GetBlackboardComponent()->GetValueAsObject(AFPAIController::TargetKey);
	//if (nullptr == IsOnTarget)
	//	return EBTNodeResult::Failed;

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AFPAIController::HomePosKey);
	FNavLocation NextPatrol;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AFPAIController::PatrolPosKey, NextPatrol.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

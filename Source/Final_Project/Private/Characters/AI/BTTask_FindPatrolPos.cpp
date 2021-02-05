// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTTask_FindPatrolPos.h"
#include "Characters/FPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigationsystem.h"

UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	// �½�ũ�� �̸��� �ٸ� �̸����� ǥ���ϰ� �ʹٸ� �� NodeName�Ӽ��� �ٸ� ������ �����ϸ� �ȴ�.
	NodeName = TEXT("FindPatrolPos");
}


/*
	ExecuteTask�Լ��� �ݵ�� �� �� �ϳ��� ���� ��ȯ�ؾ��Ѵ�.
	EBTNodeResult::Aborted	: �½�ũ ���� �߿� �ߴܵȴ�. ��������� �����ߴ�.
	EBTNodeResult::Failed	: �½�ũ�� ���������� �����ߴ�.
	EBTNodeResult::Succeeded: �½�ũ�� ���������� �����ߴ�.
	EBTNodeResult::Aborted	: �½�ũ�� ��� �����ϰ� �ִ�. �½�ũ�� ���� ����� ���� �˷��� �����̴�.

	����, �� �Լ��� ���� ��� ���� ������ �������� �ڽſ� ���� �½�ũ�� ������ ������ ��� �����ϴ� ������ �ִ�.
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

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "FPAIController.h"
#include "FPCharacter.h"


UBTTask_Attack::UBTTask_Attack() 
{
	bNotifyTick = true;
	IsAttacking = false;
}


EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto FPCharacter = Cast<AFPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == FPCharacter)
		return EBTNodeResult::Failed;

	FPCharacter->Attack();
	IsAttacking = true;
	FPCharacter->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
		});


	return EBTNodeResult::InProgress;
}

// ���⼭�� ������ �������� ƽ���� üũ���ֱ� ���� �� �Լ��� ���.
void UBTTask_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		// FinishLatentTask �Լ��� �½�ũ�� �����ٰ� �˷��ִ� �Լ��̴�. �½�ũ���� �� �Լ��� ȣ�������� ������ �����̺�� Ʈ�� �ý����� ���� �½�ũ�� ��� �ӹ��� �ȴ�. ��, ExecuteTask�� ���Ḧ �ǹ���.
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

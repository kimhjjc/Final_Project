// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "FPAIController.h"
#include "FPCharacter.h"
#include "FPMonster.h"
#include "FPSpaiderBoss.h"


UBTTask_Attack::UBTTask_Attack() 
{
	bNotifyTick = true;
	IsAttacking = false;
}


EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto FPCharacter = Cast<AFPCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	auto FPMonster = Cast<AFPMonster>(OwnerComp.GetAIOwner()->GetPawn());
	auto FPSpaiderBoss = Cast<AFPSpaiderBoss>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == FPCharacter && nullptr == FPMonster && nullptr == FPSpaiderBoss)
		return EBTNodeResult::Failed;

	if (FPSpaiderBoss)
	{
		FPLOG(Warning, TEXT("Monster Attack"));
		FPSpaiderBoss->Attack();
		IsAttacking = true;
		FPSpaiderBoss->OnAttackEnd.AddLambda([this]() -> void {
			IsAttacking = false;
			});
	}
	else if (FPMonster)
	{
		FPLOG(Warning, TEXT("Monster Attack"));
		FPMonster->Attack();
		IsAttacking = true;
		FPMonster->OnAttackEnd.AddLambda([this]() -> void {
			IsAttacking = false;
			});
	}
	else if (FPCharacter)
	{
		FPLOG(Warning, TEXT("AI Attack"));
		FPCharacter->Attack();
		IsAttacking = true;
		FPCharacter->OnAttackEnd.AddLambda([this]() -> void {
			IsAttacking = false;
			});
	}


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

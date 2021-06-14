// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/FPAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

//#include "NavigationSystem.h"
//#include "Blueprint/AIBlueprintHelperLibrary.h"

// FName�� ���·� ������������Ʈ�� � ������ ��Ī�ϴ��� �������ش�.
const FName AFPAIController::HomePosKey(TEXT("HomePos"));
const FName AFPAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AFPAIController::TargetKey(TEXT("Target"));
const FName AFPAIController::ChasingTargetKey(TEXT("ChasingTarget"));

AFPAIController::AFPAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBMonsterObject(TEXT("/Game/AI/BB_FPMonster.BB_FPMonster"));
	if (BBMonsterObject.Succeeded())
	{
		BBMonsterAsset = BBMonsterObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTMonsterObject(TEXT("/Game/AI/BT_FPMonster.BT_FPMonster"));
	if (BTMonsterObject.Succeeded())
	{
		BTMonsterAsset = BTMonsterObject.Object;
	}
	//RepeatInterval = 3.0f;
}

// AIController�� �����ϰ� �� ��.
void AFPAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

}

void AFPAIController::RUNAI()
{
	if (UseBlackboard(BBMonsterAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		Blackboard->SetValueAsInt(ChasingTargetKey, 0);
		if (!RunBehaviorTree(BTMonsterAsset))
		{
			FPLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void AFPAIController::StopAI()
{
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

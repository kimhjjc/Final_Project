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

AFPAIController::AFPAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_FPCharacter.BB_FPCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_FPCharacter.BT_FPCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

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

	//if (UseBlackboard(BBAsset, Blackboard))
	//{
	//	Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
	//	if (!RunBehaviorTree(BTAsset))
	//	{
	//		FPLOG(Error, TEXT("AIController couldn't run behavior tree!"));
	//	}
	//}




	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AFPAIController::OnRepeatTimer, RepeatInterval, true);
}

void AFPAIController::RUNAI(AFPCharacter* FPCharacter)
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			FPLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void AFPAIController::RUNAI(AFPMonster* FPMonster)
{
	if (UseBlackboard(BBMonsterAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTMonsterAsset))
		{
			FPLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}

void AFPAIController::RUNAI(AFPSpaiderBoss* FPSpaiderBoss)
{
	if (UseBlackboard(BBMonsterAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());
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

// AIController�� ������ ������ ��.
//void AFPAIController::OnUnPossess()
//{
//	Super::OnUnPossess();
//	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
//}

//void AFPAIController::OnRepeatTimer()
//{
//	auto CurrentPawn = GetPawn();
//	FPCHECK(nullptr != CurrentPawn);
//
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
//	if (nullptr == NavSystem) return;
//
//	FNavLocation NextLocation;
//	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
//	{
//		// UAIBlueprintHelperLibrary::SimpleMoveToLocation �Լ��� ���� UNavigationSystemV1�� �ִ� �Լ������� 4.20�������� ������ �ٲ�� �Ű�����.
//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
//		FPLOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
//	}
//}

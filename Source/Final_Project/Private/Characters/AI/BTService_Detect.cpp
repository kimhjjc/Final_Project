// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTService_Detect.h"
#include "Characters/FPAIController.h"
#include "Characters/Player/FPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);


	int ChasingTimer = OwnerComp.GetBlackboardComponent()->GetValueAsInt(AFPAIController::ChasingTargetKey);

	if (ChasingTimer > 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AFPAIController::ChasingTargetKey, ChasingTimer - 1);
	}
	else if (ChasingTimer == 0)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AFPAIController::TargetKey, nullptr);
	}
	

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			AFPCharacter* FPCharacter = Cast<AFPCharacter>(OverlapResult.GetActor());
			if (FPCharacter && FPCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsInt(AFPAIController::ChasingTargetKey, 10);
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AFPAIController::TargetKey, FPCharacter);

				if (FPCharacter->IsOnDrawDebug())
				{
					DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.3f);

					DrawDebugPoint(World, FPCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.3f);
					DrawDebugLine(World, ControllingPawn->GetActorLocation(), FPCharacter->GetActorLocation(), FColor::Blue, false, 0.3f);
				}
				return;


			}
		}
	}
	auto FPCharacter = Cast<AFPCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	FPCHECK(nullptr != FPCharacter);

	if (FPCharacter->IsOnDrawDebug())
	{
		DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.3f);
	}
}

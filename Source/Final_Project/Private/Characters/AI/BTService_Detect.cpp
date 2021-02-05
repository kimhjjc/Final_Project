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

	if (bResult)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AFPAIController::TargetKey, nullptr);
		for (auto OverlapResult : OverlapResults)
		{
			AFPCharacter* FPCharacter = Cast<AFPCharacter>(OverlapResult.GetActor());
			if (FPCharacter && FPCharacter->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AFPAIController::TargetKey, FPCharacter);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.3f);

				DrawDebugPoint(World, FPCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.3f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), FPCharacter->GetActorLocation(), FColor::Blue, false, 0.3f);
				return;


			}
		}
	}

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.3f);
}

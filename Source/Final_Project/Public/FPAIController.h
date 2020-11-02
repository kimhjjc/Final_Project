// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "AIController.h"
#include "FPAIController.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API AFPAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AFPAIController();
	virtual void OnPossess(APawn* InPawn) override;
	//virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	void RUNAI();
	void StopAI();

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	//void OnRepeatTimer();

	/*FTimerHandle RepeatTimerHandle;
	float RepeatInterval;*/
};

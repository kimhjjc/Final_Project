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

	
	void RUNAI(class AFPCharacter* FPCharacter);
	void RUNAI(class AFPMonster* FPMonster);
	void RUNAI(class AFPSpaiderBoss* FPSpaiderBoss);
	void RUNAI(class AFPLastBoss* FPLastBoss);
	void StopAI();

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	class UBehaviorTree* BTMonsterAsset;

	UPROPERTY()
	class UBlackboardData* BBMonsterAsset;

	//void OnRepeatTimer();

	/*FTimerHandle RepeatTimerHandle;
	float RepeatInterval;*/
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/GameStateBase.h"
#include "FPGameState.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API AFPGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	AFPGameState();
		
public:
	int32 GetTotalGameScore() const;
	void AddGameScore();

private:
	UPROPERTY(Transient)
	int32 TotalGameScore;
};

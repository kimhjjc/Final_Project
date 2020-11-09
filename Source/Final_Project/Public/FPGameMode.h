// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/GameModeBase.h"
#include "FPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API AFPGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AFPGameMode();

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void AddScore(class AFPPlayerController *ScoredPlayer);
	int32 GetScore() const;

private:
	UPROPERTY()
	class AFPGameState* FPGameState;


};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/SaveGame.h"
#include "FPSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API UFPSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UFPSaveGame();

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Exp;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 HighScore;

	UPROPERTY()
	int32 QuestNumber;
	
};

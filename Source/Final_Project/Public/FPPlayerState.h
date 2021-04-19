// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/PlayerState.h"
#include "FPPlayerState.generated.h"

/**
	1. �� Ŭ��������  �⺻������ FString PlayerName �Ӽ��� float Score �Ӽ��� �̹� ����Ǿ� �ִ�.


 */

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnLevelUpDelegate);

UCLASS()
class FINAL_PROJECT_API AFPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AFPPlayerState();

	int32 GetGameScore() const;
	int32 GetGameHighScore() const;
	FString SaveSlotName;
	int32 GetCharacterLevel() const;
	float GetExpRatio() const;
	float GetExp() const;
	float GetNextExp() const;
	bool AddExp(int32 IncomeExp);
	void AddGameScore();
	
	void InitPlayerData();
	void SavePlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;
	FOnLevelUpDelegate OnLevelUpDelegate;

protected:
	UPROPERTY(Transient)
	int GameScore;

	UPROPERTY(Transient)
	int GameHighScore;

	UPROPERTY(Transient)
	int CharacterLevel;

	UPROPERTY(Transient)
	int32 Exp;

private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FFPCharacterData* CurrentStatData;
};

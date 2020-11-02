// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerState.h"
#include "FPGameInstance.h"

AFPPlayerState::AFPPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
}

int32 AFPPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AFPPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

float AFPPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.0f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	FPLOG(Warning, TEXT("Ratio : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

bool AFPPlayerState::AddExp(int IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
		return false;

	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();
	return DidLevelUp;
}

void AFPPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	//CharacterLevel = 5;
	SetCharacterLevel(5);
	Exp = 0;
	GameScore = 0;
}

void AFPPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto FPGameInstance = Cast<UFPGameInstance>(GetGameInstance());
	FPCHECK(nullptr != FPGameInstance);

	CurrentStatData = FPGameInstance->GetFPCharacterData(NewCharacterLevel);
	FPCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}

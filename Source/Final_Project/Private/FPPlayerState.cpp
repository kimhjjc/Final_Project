// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerState.h"
#include "FPGameInstance.h"
#include "FPSaveGame.h"
#include "Effect/FPLevelUpEffect.h"
#include "UI/FPSomethingGetTextUI.h"

AFPPlayerState::AFPPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	GameHighScore = 0;
	Exp = 0;
	QuestNumber = 0;
	SaveSlotName = TEXT("Player1");
}

int32 AFPPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 AFPPlayerState::GetGameHighScore() const
{
	return GameHighScore;
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

float AFPPlayerState::GetExp() const
{
	return Exp;
}

float AFPPlayerState::GetNextExp() const
{
	return CurrentStatData->NextExp;
}
int32 AFPPlayerState::GetQuestNumber() const
{
	return QuestNumber;
}
void AFPPlayerState::SetQuestNumber(int32 newQuestNumber)
{
	QuestNumber = newQuestNumber;
	SavePlayerData();
}

bool AFPPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
		return false;

	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;

	FString AddExpText = "Exp + ";
	auto SpawnedExpGetTextUI = GetWorld()->SpawnActor<AFPSomethingGetTextUI>(AFPSomethingGetTextUI::StaticClass());
	//SpawnedExpGetTextUI->BindText(AddExpText);

	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
		OnLevelUpDelegate.Broadcast();

		auto SpawnedLevelUpEffect = GetWorld()->SpawnActor<AFPLevelUpEffect>(AFPLevelUpEffect::StaticClass());
	}

	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
	return DidLevelUp;
}

void AFPPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
}

void AFPPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Destiny"));
	//SetCharacterLevel(5);
	//Exp = 0;
	//GameScore = 0;

	// ������ �����͸� �����´�.
	auto FPSaveGame = Cast<UFPSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == FPSaveGame)
	{
		// ���� ���̺� �����Ͱ� ���ٸ� �����Ѵ�.
		FPSaveGame = GetMutableDefault<UFPSaveGame>();
	}

	SetPlayerName(FPSaveGame->PlayerName);
	SetCharacterLevel(FPSaveGame->Level);
	GameScore = 0;
	GameHighScore = FPSaveGame->HighScore;
	Exp = FPSaveGame->Exp;
	QuestNumber = FPSaveGame->QuestNumber;
	SavePlayerData();
}

void AFPPlayerState::SavePlayerData()
{
	UFPSaveGame* NewPlayerData = NewObject<UFPSaveGame>();
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->Exp = Exp;
	NewPlayerData->QuestNumber = QuestNumber;

	// ���� ���̺� �����Ͱ� ���ٸ� �����Ѵ�. ������ �״�� ����.
	if (!(UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0)))
	{
		FPLOG(Error, TEXT("SaveGame Error!"));
	}
}

void AFPPlayerState::ResetPlayerData()
{
	UFPSaveGame* NewPlayerData = NewObject<UFPSaveGame>();
	NewPlayerData->PlayerName = TEXT("Destiny");
	NewPlayerData->Level = 1;
	NewPlayerData->HighScore = 0;
	NewPlayerData->Exp = 0;
	NewPlayerData->QuestNumber = 0;

	// ���� ���̺� �����Ͱ� ���ٸ� �����Ѵ�. ������ �״�� ����.
	if (!(UGameplayStatics::SaveGameToSlot(NewPlayerData, TEXT("Player1"), 0)))
	{
		FPLOG(Error, TEXT("SaveGame Error!"));
	}
}


void AFPPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto FPGameInstance = Cast<UFPGameInstance>(GetGameInstance());
	FPCHECK(nullptr != FPGameInstance);

	CurrentStatData = FPGameInstance->GetFPCharacterData(NewCharacterLevel);
	FPCHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}

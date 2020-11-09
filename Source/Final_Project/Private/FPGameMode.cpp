// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameMode.h"
#include "FPCharacter.h"
#include "FPPlayerController.h"
#include "FPPlayerState.h"
#include "FPGameState.h"

AFPGameMode::AFPGameMode()
{
	DefaultPawnClass = AFPCharacter::StaticClass();
	PlayerControllerClass = AFPPlayerController::StaticClass();
	PlayerStateClass = AFPPlayerState::StaticClass();
	GameStateClass = AFPGameState::StaticClass();
}

void AFPGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FPGameState = Cast<AFPGameState>(GameState);
}

void AFPGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// PlayerState�� �ʱ�ȭ�� ���⿡�� ���ش�.
	auto FPPlayerState = Cast<AFPPlayerState>(NewPlayer->PlayerState);
	FPCHECK(nullptr != FPPlayerState);
	FPPlayerState->InitPlayerData();
}

void AFPGameMode::AddScore(AFPPlayerController * ScoredPlayer)
{
	// ���ӿ� �������� �÷��̾� ��Ʈ�ѷ��� ����� ���忡�� �����ϴ� GetPlayerControllerIterator�� ����� ���� �� �ִ�.
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto FPPlayerController = Cast<AFPPlayerController>(It->Get());
		if ((nullptr != FPPlayerController) && (ScoredPlayer == FPPlayerController))
		{
			FPPlayerController->AddGameScore();
			break;
		}
	}

	FPGameState->AddGameScore();
}

int32 AFPGameMode::GetScore() const
{
	return FPGameState->GetTotalGameScore();
}
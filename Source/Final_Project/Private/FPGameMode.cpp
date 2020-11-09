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

	// PlayerState의 초기화를 여기에서 해준다.
	auto FPPlayerState = Cast<AFPPlayerState>(NewPlayer->PlayerState);
	FPCHECK(nullptr != FPPlayerState);
	FPPlayerState->InitPlayerData();
}

void AFPGameMode::AddScore(AFPPlayerController * ScoredPlayer)
{
	// 게임에 참여중인 플레이어 컨트롤러의 목록은 월드에서 제공하는 GetPlayerControllerIterator를 사용해 얻어올 수 있다.
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
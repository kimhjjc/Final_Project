// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameMode.h"
#include "FPCharacter.h"
#include "FPPlayerController.h"
#include "FPPlayerState.h"

AFPGameMode::AFPGameMode()
{
	DefaultPawnClass = AFPCharacter::StaticClass();
	PlayerControllerClass = AFPPlayerController::StaticClass();
	PlayerStateClass = AFPPlayerState::StaticClass();
}

void AFPGameMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// PlayerState의 초기화를 여기에서 해준다.
	auto FPPlayerState = Cast<AFPPlayerState>(NewPlayer->PlayerState);
	FPCHECK(nullptr != FPPlayerState);
	FPPlayerState->InitPlayerData();
}

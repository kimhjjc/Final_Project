// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameState.h"

AFPGameState::AFPGameState()
{
	TotalGameScore = 0;
}

int32 AFPGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void AFPGameState::AddGameScore()
{
	TotalGameScore++;
}

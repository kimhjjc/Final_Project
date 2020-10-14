// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameMode.h"
#include "FPCharacter.h"
#include "FPPlayerController.h"

AFPGameMode::AFPGameMode()
{
	DefaultPawnClass = AFPCharacter::StaticClass();
	PlayerControllerClass = AFPPlayerController::StaticClass();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/FPUIPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "FPPlayerState.h"

void AFPUIPlayerController::ResetPlayerData()
{
	AFPPlayerState::ResetPlayerData();
}


void AFPUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FPCHECK(nullptr != UIWidgetClass);

	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	FPCHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());
	SetInputMode(Mode);
	bShowMouseCursor = true;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerController.h"
#include "FPHUDWidget.h"
#include "FPPlayerState.h"
#include "FPCharacter.h"

AFPPlayerController::AFPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UFPHUDWidget> UI_HUD_C(TEXT("/Game/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

UFPHUDWidget * AFPPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AFPPlayerController::NPCKill(AFPCharacter * KilledNPC) const
{
	FPPlayerState->AddExp(KilledNPC->GetExp());
}

void AFPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	HUDWidget = CreateWidget<UFPHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	FPPlayerState = Cast<AFPPlayerState>(PlayerState);
	FPCHECK(nullptr != FPPlayerState);
	HUDWidget->BindPlayerState(FPPlayerState);
	FPPlayerState->OnPlayerStateChanged.Broadcast();
}

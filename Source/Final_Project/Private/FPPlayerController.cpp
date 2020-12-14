// Fill out your copyright notice in the Description page of Project Settings.


#include "FPPlayerController.h"
#include "FPHUDWidget.h"
#include "FPPlayerState.h"
#include "FPCharacter.h"
#include "FPMonster.h"

AFPPlayerController::AFPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UFPHUDWidget> UI_HUD_C(TEXT("/Game/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

void AFPPlayerController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	HUDWidget = CreateWidget<UFPHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();
}

UFPHUDWidget * AFPPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

void AFPPlayerController::NPCKill(AFPCharacter * KilledNPC) const
{
	FPPlayerState->AddExp(KilledNPC->GetExp());
}

void AFPPlayerController::NPCKill(AFPMonster * KilledNPC) const
{
	FPPlayerState->AddExp(10);
}

void AFPPlayerController::NPCKill(AFPSpaiderBoss * KilledNPC) const
{
	FPPlayerState->AddExp(200);
}

void AFPPlayerController::AddGameScore() const
{
	FPPlayerState->AddGameScore();
}

void AFPPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// 패키징 오류로 인하여 순서를 앞부분으로 바꿔준다. (OnPossess로)
	// https://m.blog.naver.com/PostView.nhn?blogId=uriel1234&logNo=221539024076&proxyReferer=https:%2F%2Fwww.google.com%2F 참조
	/*HUDWidget = CreateWidget<UFPHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();*/

	FPPlayerState = Cast<AFPPlayerState>(PlayerState);
	FPCHECK(nullptr != FPPlayerState);
	HUDWidget->BindPlayerState(FPPlayerState);
	FPPlayerState->OnPlayerStateChanged.Broadcast();
}

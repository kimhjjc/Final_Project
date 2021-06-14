// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/FPPlayerController.h"
#include "UI/FPHUDWidget.h"
#include "UI/FPQuestWidget.h"
#include "UI/FPConversationWidget.h"
#include "UI/FPStatusWindowWidget.h"
#include "FPPlayerState.h"
#include "Characters/Player/FPCharacter.h"
#include "Characters/Enemy/FPMonster.h"

AFPPlayerController::AFPPlayerController()
{
	static ConstructorHelpers::FClassFinder<UFPHUDWidget> UI_HUD_C(TEXT("/Game/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UFPQuestWidget> UI_QUEST_C(TEXT("/Game/UI/UI_QuestWindow.UI_QuestWindow_C"));
	if (UI_QUEST_C.Succeeded())
	{
		QuestWidgetClass = UI_QUEST_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UFPConversationWidget> UI_CONTENT_C(TEXT("/Game/UI/UI_Conversation.UI_Conversation_C"));
	if (UI_CONTENT_C.Succeeded())
	{
		ConversationWidgetClass = UI_CONTENT_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UFPStatusWindowWidget> UI_STATUS_C(TEXT("/Game/UI/UI_StatusWindow.UI_StatusWindow_C"));
	if (UI_STATUS_C.Succeeded())
	{
		StatusWindowWidgetClass = UI_STATUS_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UI_TUTORIAL_C(TEXT("/Game/UI/TutorialUI/UI_HowToMove.UI_HowToMove_C"));
	if (UI_TUTORIAL_C.Succeeded())
	{
		TutorialWindowWidgetClass = UI_TUTORIAL_C.Class;
	}
}

void AFPPlayerController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	HUDWidget = CreateWidget<UFPHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	QuestWidget = CreateWidget<UFPQuestWidget>(this, QuestWidgetClass);
	QuestWidget->AddToViewport();
	QuestWidget->SetVisibility(ESlateVisibility::Collapsed);

	ConversationWidget = CreateWidget<UFPConversationWidget>(this, ConversationWidgetClass);
	ConversationWidget->AddToViewport();
	ConversationWidget->SetVisibility(ESlateVisibility::Collapsed);

	StatusWindowWidget = CreateWidget<UFPStatusWindowWidget>(this, StatusWindowWidgetClass);
	StatusWindowWidget->AddToViewport();
	StatusWindowWidget->SetVisibility(ESlateVisibility::Collapsed);

	TutorialWindowWidget = CreateWidget<UUserWidget>(this, TutorialWindowWidgetClass);
	TutorialWindowWidget->AddToViewport();
	TutorialWindowWidget->SetVisibility(ESlateVisibility::Collapsed);
}

UFPHUDWidget * AFPPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

UFPQuestWidget * AFPPlayerController::GetQuestWidget() const
{
	return QuestWidget;
}

UFPConversationWidget * AFPPlayerController::GetConversationWidget() const
{
	return ConversationWidget;
}

UFPStatusWindowWidget * AFPPlayerController::GetStatusWindowWidget() const
{
	return StatusWindowWidget;
}

UUserWidget * AFPPlayerController::GetTutorialWindowWidget() const
{
	return TutorialWindowWidget;
}

void AFPPlayerController::SetTutorialWindowWidget(TSubclassOf<class UUserWidget> NewWindowWidgetClass)
{
	TutorialWindowWidget = CreateWidget<UUserWidget>(this, NewWindowWidgetClass);
	TutorialWindowWidget->AddToViewport();
	TutorialWindowWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AFPPlayerController::NPCKill(AFPCharacter * KilledNPC) const
{
	FPPlayerState->AddExp(KilledNPC->GetExp());
	if (FPPlayerState->GetQuestNumber() == 4)
	{
		OnQuestUpdate.Broadcast();
	}
}

void AFPPlayerController::NPCKill(AFPMonster * KilledNPC) const
{
	FPPlayerState->AddExp(10);
	if (FPPlayerState->GetQuestNumber() == 2)
	{
		OnQuestUpdate.Broadcast();
	}
}

void AFPPlayerController::NPCKill(AFPSpaiderBoss * KilledNPC) const
{
	FPPlayerState->AddExp(200);
	if (FPPlayerState->GetQuestNumber() == 5)
	{
		OnQuestUpdate.Broadcast();
	}
}

void AFPPlayerController::NPCKill(AFPLastBoss * KilledNPC) const
{
	FPPlayerState->AddExp(400);
	if (FPPlayerState->GetQuestNumber() == 6)
	{
		OnQuestUpdate.Broadcast();
	}
}

void AFPPlayerController::NPCKill(int32 Exp) const
{
	FPPlayerState->AddExp(Exp);
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

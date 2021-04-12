// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/FPTutorialNPC_NextQuestPosition.h"

AFPTutorialNPC_NextQuestPosition::AFPTutorialNPC_NextQuestPosition()
{
	QuestNumber = 0;
	
}

void AFPTutorialNPC_NextQuestPosition::BeginPlay()
{
	Super::BeginPlay();

	if (GetName() == "FPTutorialNPC_NextQuestPosition1")
		QuestNumber = 1;
}

void AFPTutorialNPC_NextQuestPosition::SetQuestNumber(int32 QuestNumber_)
{
	QuestNumber = QuestNumber_;
}


int32 AFPTutorialNPC_NextQuestPosition::GetQuestNumber()
{
	return QuestNumber;
}
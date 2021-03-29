// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPQuestWidget.h"
#include "Components/TextBlock.h"


void UFPQuestWidget::BindNPCQuest(FString NPCName_F, FString QuestInfo_F)
{
	NPCName->SetText(FText::FromString(NPCName_F));
	QuestInfo->SetText(FText::FromString(QuestInfo_F));
}

void UFPQuestWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NPCName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtNPCName")));
	FPCHECK(nullptr != NPCName);

	QuestInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtQuestInfo")));
	FPCHECK(nullptr != QuestInfo);

	NPCName->SetText(FText::FromString("NPC"));
	QuestInfo->SetText(FText::FromString("QuestInfo"));
}
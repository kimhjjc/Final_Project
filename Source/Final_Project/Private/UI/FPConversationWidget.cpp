// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPConversationWidget.h"
#include "Components/TextBlock.h"


void UFPConversationWidget::BindNPCContent(FString NPCName_F, FString Content_F)
{
	NPCName->SetText(FText::FromString(NPCName_F));
	Content->SetText(FText::FromString(Content_F));
}

void UFPConversationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NPCName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtNPCName")));
	FPCHECK(nullptr != NPCName);

	Content = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtContent")));
	FPCHECK(nullptr != Content);

	NPCName->SetText(FText::FromString("NPC"));
	Content->SetText(FText::FromString("Content"));
}
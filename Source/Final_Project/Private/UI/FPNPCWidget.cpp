// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPNPCWidget.h"
#include "Components/TextBlock.h"


void UFPNPCWidget::BindNPCName(FString NPCName_F)
{
	NPCName->SetText(FText::FromString(NPCName_F));
}

void UFPNPCWidget::NativeConstruct()
{
	Super::NativeConstruct();
	NPCName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtNPCName")));
	FPCHECK(nullptr != NPCName);
	NPCName->SetText(FText::FromString("Tutorial"));
}
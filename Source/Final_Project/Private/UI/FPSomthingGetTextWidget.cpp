// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPSomthingGetTextWidget.h"
#include "Components/TextBlock.h"


void UFPSomthingGetTextWidget::BindWantText(FString WantText)
{
	AcquiredString = WantText;
	AcquiredText->SetText(FText::FromString(WantText));
}

void UFPSomthingGetTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AcquiredText = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtAcquiredText")));
	FPCHECK(nullptr != AcquiredText);
	AcquiredText->SetText(FText::FromString("Exp + "));
}
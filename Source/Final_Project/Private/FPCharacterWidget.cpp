// Fill out your copyright notice in the Description page of Project Settings.


#include "FPCharacterWidget.h"
#include "FPCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UFPCharacterWidget::BindCharacterStat(class UFPCharacterStatComponent* NewCharacterStat)
{
	FPCHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UFPCharacterWidget::UpdateHPWidget);
}

void UFPCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	FPCHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UFPCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FPStatusWindowWidget.h"
#include "Components/TextBlock.h"


void UFPStatusWindowWidget::BindCharacterStat(int32 Lv, int32 Atk, int32 Def, int32 Hp, int32 Exp)
{
	Level->SetText(FText::FromString(FString::FromInt(Lv)));
	Attack->SetText(FText::FromString(FString::FromInt(Atk)));
	Defence->SetText(FText::FromString(FString::FromInt(Def)));
	HP->SetText(FText::FromString(FString::FromInt(Hp)));
	EXP->SetText(FText::FromString(FString::FromInt(Exp)));
}

void UFPStatusWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Level = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLv_Stat")));
	FPCHECK(nullptr != Level);
	
	Attack = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtATK_Stat")));
	FPCHECK(nullptr != Attack);

	Defence = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtDEF_Stat")));
	FPCHECK(nullptr != Defence);

	HP = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHP_Stat")));
	FPCHECK(nullptr != HP);

	EXP = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtEXP_Stat")));
	FPCHECK(nullptr != EXP);
}
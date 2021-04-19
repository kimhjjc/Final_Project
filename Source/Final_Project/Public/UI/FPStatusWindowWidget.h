// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Blueprint/UserWidget.h"
#include "FPStatusWindowWidget.generated.h"

/**
 *
 */

DECLARE_MULTICAST_DELEGATE(FOnStatusUpdateDelegate);

UCLASS()
class FINAL_PROJECT_API UFPStatusWindowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(int32 Lv, int32 Atk, int32 Def, int32 Hp, int32 MaxHp, int32 Exp, int32 NextExp);

	FOnStatusUpdateDelegate OnStatusUpdate;

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UTextBlock* Level;

	UPROPERTY()
	class UTextBlock* Attack;

	UPROPERTY()
	class UTextBlock* Defence;

	UPROPERTY()
	class UTextBlock* HP;

	UPROPERTY()
	class UTextBlock* EXP;
};
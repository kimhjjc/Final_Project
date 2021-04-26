// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Blueprint/UserWidget.h"
#include "FPSomthingGetTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API UFPSomthingGetTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindWantText(FString WantText);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UTextBlock* AcquiredText;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	FString AcquiredString;
};

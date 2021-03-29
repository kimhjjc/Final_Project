// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Blueprint/UserWidget.h"
#include "FPQuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API UFPQuestWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindNPCQuest(FString NPCName_F, FString QuestInfo_F);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
		class UTextBlock* NPCName;

	UPROPERTY()
		class UTextBlock* QuestInfo;
};

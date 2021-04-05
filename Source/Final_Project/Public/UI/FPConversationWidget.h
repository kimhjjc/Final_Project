// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Blueprint/UserWidget.h"
#include "FPConversationWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API UFPConversationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindNPCContent(FString NPCName_F, FString Content_F);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
		class UTextBlock* NPCName;

	UPROPERTY()
		class UTextBlock* Content;
};

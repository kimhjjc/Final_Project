// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Blueprint/UserWidget.h"
#include "FPNPCWidget.generated.h"

/**
 *
 */
UCLASS()
class FINAL_PROJECT_API UFPNPCWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindNPCName(FString NPCName_F);

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	class UTextBlock* NPCName;
};

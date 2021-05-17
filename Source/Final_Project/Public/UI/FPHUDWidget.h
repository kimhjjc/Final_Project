// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Blueprint/UserWidget.h"
#include "FPHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API UFPHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UFPCharacterStatComponent* CharacterStat);
	void BindPlayerState(class AFPPlayerState* PlayerState);
	void UpdateQuest(FString QuestInfo_F);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UFPCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class AFPPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* PlayerName;

	UPROPERTY()
	class UTextBlock* PlayerLevel;

	UPROPERTY()
	class UTextBlock* CurrentScore;

	UPROPERTY()
	class UTextBlock* HighScore;

	UPROPERTY()
	class UTextBlock* QuestInfo;

};

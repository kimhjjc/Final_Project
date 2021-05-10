// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/PlayerController.h"
#include "FPPlayerController.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnQuestUpdateDelegate);

UCLASS()
class FINAL_PROJECT_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFPPlayerController();
	virtual void OnPossess(APawn* InPawn) override;

	class UFPHUDWidget* GetHUDWidget() const;
	class UFPQuestWidget* GetQuestWidget() const;
	class UFPConversationWidget* GetConversationWidget() const;
	class UFPStatusWindowWidget* GetStatusWindowWidget() const;
	void NPCKill(class AFPCharacter* KilledNPC) const;
	void NPCKill(class AFPMonster* KilledNPC) const; 
	void NPCKill(class AFPSpaiderBoss* KilledNPC) const;
	void NPCKill(class AFPLastBoss* KilledNPC) const;
	void NPCKill(int32 Exp) const;
	void AddGameScore() const;

	class AFPPlayerState* GetFPPlayerState() { return FPPlayerState; }
	FOnQuestUpdateDelegate OnQuestUpdate;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UFPHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UFPQuestWidget> QuestWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UFPConversationWidget> ConversationWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UFPStatusWindowWidget> StatusWindowWidgetClass;

private:
	UPROPERTY()
	class UFPHUDWidget* HUDWidget;

	UPROPERTY()
	class UFPQuestWidget* QuestWidget;

	UPROPERTY()
	class UFPConversationWidget* ConversationWidget;

	UPROPERTY()
	class UFPStatusWindowWidget* StatusWindowWidget;

	UPROPERTY()
	class AFPPlayerState* FPPlayerState;
	
};

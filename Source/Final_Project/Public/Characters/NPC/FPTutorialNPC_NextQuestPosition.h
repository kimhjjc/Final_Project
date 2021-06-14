// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Engine/TriggerBox.h"
#include "FPTutorialNPC_NextQuestPosition.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API AFPTutorialNPC_NextQuestPosition : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPTutorialNPC_NextQuestPosition();

	void SetQuestNumber(int32 QuestNumber_);
	int32 GetQuestNumber();


	UPROPERTY(EditAnywhere, Category = Quest)
	int32 QuestNumber;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Quest, Meta = (AllowPrivateAccess = true))
	//int32 QuestNumber;
	
};

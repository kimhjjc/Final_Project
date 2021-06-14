// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/PlayerController.h"
#include "FPUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API AFPUIPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Restart")
	void ResetPlayerData();

private:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = UI, Meta = (AllowPrivateAccess = true))
	TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
	class UUserWidget* UIWidgetInstance;


};

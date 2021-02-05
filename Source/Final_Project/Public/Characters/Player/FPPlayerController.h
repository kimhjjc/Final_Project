// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/PlayerController.h"
#include "FPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API AFPPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AFPPlayerController();
	virtual void OnPossess(APawn* InPawn) override;

	class UFPHUDWidget* GetHUDWidget() const;
	void NPCKill(class AFPCharacter* KilledNPC) const;
	void NPCKill(class AFPMonster* KilledNPC) const;
	void NPCKill(class AFPSpaiderBoss* KilledNPC) const;
	void AddGameScore() const;

	class AFPPlayerState* GetFPPlayerState() { return FPPlayerState; }
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UFPHUDWidget> HUDWidgetClass;

private:
	UPROPERTY()
	class UFPHUDWidget* HUDWidget;

	UPROPERTY()
	class AFPPlayerState* FPPlayerState;
};

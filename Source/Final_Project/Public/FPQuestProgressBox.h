// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Actor.h"
#include "FPQuestProgressBox.generated.h"

UCLASS()
class FINAL_PROJECT_API AFPQuestProgressBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPQuestProgressBox();

	UPROPERTY(VisibleAnywhere, Category = Box)
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, Category = Quest)
	int32 QuestNumber;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> HowToMoveWindowWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> TutorialWindowWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UUserWidget> HowToAttackWindowWidgetClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	void QuestUpdate(class AFPCharacter* FPCharacter, bool bIsGameLoad = false);
	void CloseContentWidget();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	FString QuestInfo;

};

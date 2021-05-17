// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Character.h"
#include "FPTutorialNPC.generated.h"

UCLASS()
class FINAL_PROJECT_API AFPTutorialNPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPTutorialNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* NPCNameWidget;

	UPROPERTY(VisibleAnywhere, Category = UI)
	class UWidgetComponent* QuestCheckWidget;

	UPROPERTY(VisibleAnywhere, Category = Box)
	UBoxComponent* Trigger;

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	FString NPCName;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	int32 QuestNumber;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	FString QuestInfo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	int32 MonsterKill;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	int32 TargetKill;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	bool IsQuestAccept;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	FString ContentInfo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = NPC, Meta = (AllowPrivateAccess = true))
	bool IsInteractive;

	// ���̳��� �Լ��� UFUNCTION()�� ���� ������ ������� ����.
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CloseContentWidget();

	FTimerHandle CloseTimerHandle = { };
};

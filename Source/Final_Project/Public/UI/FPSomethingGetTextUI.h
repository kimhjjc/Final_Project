// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "GameFramework/Actor.h"
#include "FPSomethingGetTextUI.generated.h"

UCLASS()
class FINAL_PROJECT_API AFPSomethingGetTextUI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSomethingGetTextUI();
	void BindText(FString Text);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle TimerHandle = { };

	bool TransformStart;
	float TextZLocation;

	class UWidgetComponent* TextWidget;
	FString MyText;
};

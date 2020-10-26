// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Blueprint/UserWidget.h"
#include "FPCharacterWidget.generated.h"

/**
	1. 위젯 블루프린트가 사용하는 기반 C__ 클래스는 UserWidget이다.
 */
UCLASS()
class FINAL_PROJECT_API UFPCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UFPCharacterStatComponent* NewCharacterStat);

protected:
	// NativeConstruct는 UI 시스템이 준비되면 호출되는 함수이다.
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	// 약포인터는 원래 서로 다른 액터라면 사용하는 것이 바람직하다. -->>TWeakOjbectgPtr
	TWeakObjectPtr<class UFPCharacterStatComponent> CurrentCharacterStat;
	
	UPROPERTY()
	class UProgressBar* HPProgressBar;
};

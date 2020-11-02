// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Blueprint/UserWidget.h"
#include "FPCharacterWidget.generated.h"

/**
	1. ���� �������Ʈ�� ����ϴ� ��� C__ Ŭ������ UserWidget�̴�.
 */
UCLASS()
class FINAL_PROJECT_API UFPCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UFPCharacterStatComponent* NewCharacterStat);

protected:
	// NativeConstruct�� UI �ý����� �غ�Ǹ� ȣ��Ǵ� �Լ��̴�.
	virtual void NativeConstruct() override;
	void UpdateHPWidget();

private:
	// �������ʹ� ���� ���� �ٸ� ���Ͷ�� ����ϴ� ���� �ٶ����ϴ�. -->>TWeakOjbectgPtr
	TWeakObjectPtr<class UFPCharacterStatComponent> CurrentCharacterStat;
	
	UPROPERTY()
	class UProgressBar* HPProgressBar;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Detect.generated.h"

/**
 	���� ����, ���������� �������� �ʰ� ������ ��忡 �����Ǵ� ����.
	����, �ش� �������� ���� �½�ũ���� ����Ǵ� ���� �ݺ����� �۾��� �����ϴ� �� �����ϴ�.
 */
UCLASS()
class FINAL_PROJECT_API UBTService_Detect : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_Detect();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};

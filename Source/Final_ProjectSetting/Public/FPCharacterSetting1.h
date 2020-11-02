// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPCharacterSetting1.generated.h"

/**
	1. �ּ��� �����ڿ��� �ڵ�� ������ �� ���� ������, �� ��� �ּ��� ����Ǹ� �ڵ带 �ٽ� ����� ������ �ؾ��ϴ� ���ŷο��� �ִ�. ������ config�� .ini ������ �̿��ϸ� �⺻ �Ӽ� ���� �����Ͽ� �����ϰ� ������ �� �ְ� ���ش�.

 */

 // UCLASS()�ȿ� �Ʒ��� ���� ����Ͽ� DefaultFinal_Project.ini�� �ҷ��´�.
UCLASS(config=Final_Project)
class FINAL_PROJECTSETTING_API UFPCharacterSetting1 : public UObject
{
	GENERATED_BODY()

public:
	UFPCharacterSetting1();

	// �ּ� ��θ� �����ϱ� ���� Ŭ������ �ٷ� FSoftObjectPath Ŭ������.
	// �Ʒ� UPROPERTY(config)�� ���� CharacterAssets���� config�� DefaultFinal_Project.ini ������ �����ϰ� �ȴ�.
	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};

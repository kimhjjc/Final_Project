// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Engine/DataTable.h"	// ������ ���̺��� ����ϱ� ���� ����
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "FPGameInstance.generated.h"

/**
	1. �𸮾󿡼��� ���� ���� �߿����� �񵿱� ������� �ּ��� �ε��ϵ��� FStreamableManager��� Ŭ������ �����Ѵ�.
		- �� �Ŵ��� Ŭ������ ������Ʈ���� �� �ϳ��� Ȱ��ȭ �ϴ� ���� ���� ������ ������ �ν��Ͻ��� �����ϴ� FPGameInstance Ŭ������ �̸� ��� ������ �����Ѵ�.
		- �񵿱� ������� �ε��ϴ� ����� AsyncLoad�̰�, FStreamableDelegate ������ ��������Ʈ�� �Ѱ��� ���, �ּ��� �ε��� �Ϸ��ϸ� �ش� ��������Ʈ�� ����� �Լ��� ȣ�����ش�.
		- �ٸ� ������� FStreamableDelegate �������� ��������Ʈ ����� �������� �ʰ� CreateUObject����� ����� �Ｎ���� ��������Ʈ�� ���������ν� �Լ��� ������Ų �� �Ѱ��ִ� ����� �� �����ϴ�. FPCharacter.cpp�� OnAssetLoadCompleted() ����.
	
	2. �� FStreamableManager�� ���� ��� ���� �����ϴ� UAssetManager��� ������Ʈ�� �̹� ����� �ִ�. Engine/AssetManager.h ��� ������ ������ �� UAssetManager::GetStreamableManager() �Լ��� ��� ����ص� �����ϴ�.
 
 */

// �𸮾󿡼� CVS������ ����Ʈ ��Ű�� ���ؼ��� �̿Ͱ��� ����ü�� ������ �־�� ����������.
USTRUCT(BlueprintType)
struct FFPCharacterData : public FTableRowBase
{
	GENERATED_BODY()


public:
	// CVS���� ù��° ���� Name ���� �𸮾� �������� �ڵ����� Ű ������ ����ϱ� ������ Name ���� ���𿡼� �����Ѵ�.
	FFPCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

UCLASS()
class FINAL_PROJECT_API UFPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFPGameInstance();

	virtual void Init() override;
	FFPCharacterData* GetFPCharacterData(int32 Level);

	// �� Ŭ������ ���� ������ �� �ϳ��� �����ϴ� ���� ����.
	FStreamableManager StreamableManager;

private:
	UPROPERTY()
	class UDataTable* FPCharacterTable;
};

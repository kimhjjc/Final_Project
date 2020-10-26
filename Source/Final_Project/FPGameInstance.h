// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Engine/DataTable.h"	// 데이터 테이블을 사용하기 위한 선언
#include "Engine/GameInstance.h"
#include "FPGameInstance.generated.h"

/**
 * 
 */

// 언리얼에서 CVS파일을 임포트 시키기 위해서는 이와같이 구조체를 선언해 주어야 가능해진다.
USTRUCT(BlueprintType)
struct FFPCharacterData : public FTableRowBase
{
	GENERATED_BODY()


public:
	// CVS에서 첫번째 열인 Name 열은 언리얼 엔진에서 자동으로 키 값으로 사용하기 때문에 Name 열은 선언에서 제외한다.
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

private:
	UPROPERTY()
	class UDataTable* FPCharacterTable;
};

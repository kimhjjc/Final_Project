// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Engine/DataTable.h"	// 데이터 테이블을 사용하기 위한 선언
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "FPGameInstance.generated.h"

/**
	1. 언리얼에서는 게임 진행 중에서도 비동기 방식으로 애셋을 로딩하도록 FStreamableManager라는 클래스를 제공한다.
		- 이 매니저 클래스는 프로젝트에서 단 하나만 활성화 하는 것이 좋기 때문에 유일한 인스턴스로 동작하는 FPGameInstance 클래스에 이를 멤버 변수로 선언한다.
		- 비동기 방식으로 로딩하는 명령은 AsyncLoad이고, FStreamableDelegate 형식의 델리게이트로 넘겨줄 경우, 애셋이 로딩을 완료하면 해당 델리게이트에 연결된 함수를 호출해준다.
		- 다른 방법으로 FStreamableDelegate 형식으로 델리게이트 멤버를 선언하지 않고 CreateUObject명령을 사용해 즉석에서 델리게이트를 생성함으로써 함수와 연동시킨 후 넘겨주는 방식이 더 간편하다. FPCharacter.cpp의 OnAssetLoadCompleted() 참조.
	
	2. 이 FStreamableManager는 엔진 모듈 내에 존재하는 UAssetManager라는 오브젝트에 이미 선언돼 있다. Engine/AssetManager.h 헤더 파일을 포함한 후 UAssetManager::GetStreamableManager() 함수를 대신 사용해도 무방하다.
 
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

	// 이 클래스는 게임 내에서 단 하나만 생성하는 것이 좋다.
	FStreamableManager StreamableManager;

private:
	UPROPERTY()
	class UDataTable* FPCharacterTable;
};

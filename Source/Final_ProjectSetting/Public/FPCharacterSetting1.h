// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FPCharacterSetting1.generated.h"

/**
	1. 애셋은 생성자에서 코드로 지정해 줄 수도 있지만, 이 경우 애셋이 변경되면 코드를 다시 만들고 컴파일 해야하는 번거로움이 있다. 하지만 config의 .ini 파일을 이용하면 기본 속성 값만 지정하여 유연하게 관리할 수 있게 해준다.

 */

 // UCLASS()안에 아래와 같이 사용하여 DefaultFinal_Project.ini를 불러온다.
UCLASS(config=Final_Project)
class FINAL_PROJECTSETTING_API UFPCharacterSetting1 : public UObject
{
	GENERATED_BODY()

public:
	UFPCharacterSetting1();

	// 애셋 경로를 보관하기 위한 클래스가 바로 FSoftObjectPath 클래스다.
	// 아래 UPROPERTY(config)로 인해 CharacterAssets에서 config의 DefaultFinal_Project.ini 내용을 저장하게 된다.
	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};

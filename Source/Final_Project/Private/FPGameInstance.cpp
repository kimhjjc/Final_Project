// Fill out your copyright notice in the Description page of Project Settings.


#include "FPGameInstance.h"

UFPGameInstance::UFPGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/GameData/FPCharacterData.FPCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_FPCHARACTER(*CharacterDataPath);
	FPCHECK(DT_FPCHARACTER.Succeeded());
	FPCharacterTable = DT_FPCHARACTER.Object;
	//FPCHECK(FPCharacterTable->RowMap.Num() > 0);
}

void UFPGameInstance::Init()
{
	Super::Init();
	//FPLOG(Warning, TEXT("DropExp of Level 20 FPCharacter : %d"), GetFPCharacterData(20)->DropExp);
}

FFPCharacterData * UFPGameInstance::GetFPCharacterData(int32 Level)
{
	return FPCharacterTable->FindRow<FFPCharacterData>(*FString::FromInt(Level), TEXT(""));
}

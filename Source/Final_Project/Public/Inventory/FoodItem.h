// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Final_Project.h"
#include "Inventory/Item.h"
#include "FoodItem.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_PROJECT_API UFoodItem : public UItem
{
	GENERATED_BODY()
	
public:

	// The weight of the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (ClampMin = 0.0f))
	float HealthToHeal;

protected:

	virtual void Use(class AFPCharacter* Character) override;
};

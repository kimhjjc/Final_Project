// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/FoodItem.h"
#include "Inventory/InventoryComponent.h"
#include "Characters/Player/FPCharacter.h"

void UFoodItem::Use(AFPCharacter * Character)
{
	if (Character)
	{
		Character->Health += HealthToHeal;

		if (OwningInventory)
		{
			OwningInventory->RemoveItem(this);
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Component/InventoryComponent.h"

#include "ItemInteraction.generated.h"

class AItemsBase;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UItemInteraction : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE AItemsBase* PlayerOverLapItem() const
	{
		/*if (UReferenceHelper::InventoryComponent)
		{
			return UReferenceHelper::InventoryComponent->OverlapItem;	
		}*/
		return nullptr;
	}
};

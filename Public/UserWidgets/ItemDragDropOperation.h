// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

enum class ESlotTypes : uint8;
class USlotsHandler;
class UInventoryComponent;

/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	static UItemDragDropOperation* Create();

	UPROPERTY(EditAnywhere)
	ESlotTypes SlotType;
	
	UPROPERTY()
	USlotsHandler* SlotsHandler;

	UPROPERTY()
	bool IsSplit;

	UPROPERTY()
	int32 SplitCount;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InventorySupport.generated.h"

struct FInventoryItems;
class UCharacterSlot;
class AInventoryPlayerController;
class UStorageSlot;
class USlotsHandler;
class UInventorySlot;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UInventorySupport : public UObject
{
	GENERATED_BODY()
public:
	static void Swap(TObjectPtr<AInventoryPlayerController> InventoryPlayerController, TObjectPtr<UInventorySlot> InventorySlot , TObjectPtr<USlotsHandler> SlotsHandler);
	static void Swap(TObjectPtr<AInventoryPlayerController> InventoryPlayerController, TObjectPtr<UStorageSlot> StorageSlot , TObjectPtr<USlotsHandler> SlotsHandler);
	static void Swap(TObjectPtr<AInventoryPlayerController> InventoryPlayerController, TObjectPtr<UCharacterSlot> CharacterSlot , TObjectPtr<USlotsHandler> SlotsHandler);

private:
	static void ChooseSlotType(TObjectPtr<AInventoryPlayerController> InventoryPlayerController, TObjectPtr<USlotsHandler> SlotsHandler , TTuple<FInventoryItems*, int32, int32> InventoryItems);
	static void HandleInventorySlot(TObjectPtr<AInventoryPlayerController> InventoryPlayerController, TObjectPtr<UInventorySlot> InventorySlot , TTuple<FInventoryItems*, int32, int32> InventoryItems);
	static void HandleStorageSlot(TObjectPtr<AInventoryPlayerController> InventoryPlayerController, TObjectPtr<UStorageSlot> StorageSlot , TTuple<FInventoryItems*, int32, int32> InventoryItems);
	static void HandleCharacterSlot(TObjectPtr<AInventoryPlayerController> InventoryPlayerController, TObjectPtr<UCharacterSlot> CharacterSlot , TTuple<FInventoryItems*, int32, int32> InventoryItems);
};

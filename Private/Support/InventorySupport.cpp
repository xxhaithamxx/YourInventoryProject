// Fill out your copyright notice in the Description page of Project Settings.


#include "Support/InventorySupport.h"

#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/Inventory/StoragePanel.h"
#include "UserWidgets/Inventory/Slots/CharacterSlot.h"
#include "UserWidgets/Inventory/Slots/InventorySlot.h"
#include "UserWidgets/Inventory/Slots/StorageSlot.h"

void UInventorySupport::Swap(TObjectPtr<AInventoryPlayerController> InventoryPlayerController,
                             TObjectPtr<UInventorySlot> InventorySlot, TObjectPtr<USlotsHandler> SlotsHandler)
{
	if (SlotsHandler->IsItemValid())
	{
		if (InventorySlot->IsItemValid())
		{
			TTuple<FInventoryItems*, int32, int32> SwapInventoryItem = InventorySlot->GetInventoryItems();
			HandleInventorySlot(InventoryPlayerController, InventorySlot, SlotsHandler->GetInventoryItems());
			ChooseSlotType(InventoryPlayerController, SlotsHandler, SwapInventoryItem);
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("UInventorySupport::Swap 1"));
		HandleInventorySlot(InventoryPlayerController, InventorySlot, SlotsHandler->GetInventoryItems());
		UE_LOG(LogTemp, Warning, TEXT("UInventorySupport::Swap 2"));
		SlotsHandler->ClearSlot();
	}
}

void UInventorySupport::Swap(TObjectPtr<AInventoryPlayerController> InventoryPlayerController,
                             TObjectPtr<UStorageSlot> StorageSlot, TObjectPtr<USlotsHandler> SlotsHandler)
{
	if (SlotsHandler->IsItemValid())
	{
		if (StorageSlot->IsItemValid())
		{
			TTuple<FInventoryItems*, int32, int32> SwapInventoryItem = StorageSlot->GetInventoryItems();
			HandleStorageSlot(InventoryPlayerController, StorageSlot, SlotsHandler->GetInventoryItems());
			ChooseSlotType(InventoryPlayerController, SlotsHandler, SwapInventoryItem);
			return;
		}
		UE_LOG(LogTemp, Warning, TEXT("UInventorySupport::Swap 1"));
		HandleStorageSlot(InventoryPlayerController, StorageSlot, SlotsHandler->GetInventoryItems());
		UE_LOG(LogTemp, Warning, TEXT("UInventorySupport::Swap 2"));
		SlotsHandler->ClearSlot();
	}
}


void UInventorySupport::Swap(TObjectPtr<AInventoryPlayerController> InventoryPlayerController,
                             TObjectPtr<UCharacterSlot> CharacterSlot, TObjectPtr<USlotsHandler> SlotsHandler)
{
	TTuple<FInventoryItems*, int32, int32> SwapInventoryItem = CharacterSlot->GetInventoryItems();
	HandleCharacterSlot(InventoryPlayerController, CharacterSlot, SlotsHandler->GetInventoryItems());
	ChooseSlotType(InventoryPlayerController, SlotsHandler, SwapInventoryItem);
}

void UInventorySupport::ChooseSlotType(TObjectPtr<AInventoryPlayerController> InventoryPlayerController,
                                       TObjectPtr<USlotsHandler> SlotsHandler,
                                       TTuple<FInventoryItems*, int32, int32> InventoryItems)
{
	switch (SlotsHandler->GetSlotType())
	{
	case ESlotTypes::EInventory:
		HandleInventorySlot(InventoryPlayerController, Cast<UInventorySlot>(SlotsHandler), InventoryItems);
		break;
	case ESlotTypes::ECharacter:
		HandleCharacterSlot(InventoryPlayerController, Cast<UCharacterSlot>(SlotsHandler), InventoryItems);
		break;
	case ESlotTypes::EBank:
		HandleStorageSlot(InventoryPlayerController, Cast<UStorageSlot>(SlotsHandler), InventoryItems);
		break;
	case ESlotTypes::ESkills:
		break;
	case ESlotTypes::EActionBar:
		break;
	}
}

void UInventorySupport::HandleInventorySlot(TObjectPtr<AInventoryPlayerController> InventoryPlayerController,
                                            TObjectPtr<UInventorySlot> InventorySlot,
                                            TTuple<FInventoryItems*, int32, int32> InventoryItems)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventorySupport::HandleInventorySlot 1"));
	if (InventorySlot->IsItemValid())
	{
		InventorySlot->ClearSlot();
	}
	InventorySlot->AddItem(InventoryItems);
	InventoryPlayerController->GetInventoryPanel()->AddSlot(InventorySlot);
}

void UInventorySupport::HandleStorageSlot(TObjectPtr<AInventoryPlayerController> InventoryPlayerController,
                                          TObjectPtr<UStorageSlot> StorageSlot,
                                          TTuple<FInventoryItems*, int32, int32> InventoryItems)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventorySupport::HandleStorageSlot 1"));
	if (StorageSlot->IsItemValid())
	{
		StorageSlot->ClearSlot();
	}
	StorageSlot->AddItem(InventoryItems);
	InventoryPlayerController->GetStoragePanel()->AddSlot(StorageSlot);
}

void UInventorySupport::HandleCharacterSlot(TObjectPtr<AInventoryPlayerController> InventoryPlayerController,
                                            TObjectPtr<UCharacterSlot> CharacterSlot,
                                            TTuple<FInventoryItems*, int32, int32> InventoryItems)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventorySupport::HandleCharacterSlot 1"));
	if (CharacterSlot->IsItemValid())
	{
		CharacterSlot->ClearSlot();
	}
	CharacterSlot->AddItem(InventoryItems);
}

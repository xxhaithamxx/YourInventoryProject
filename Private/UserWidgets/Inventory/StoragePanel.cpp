// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/StoragePanel.h"

#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Items/StorageController.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"
#include "Structs/SlotDataArray.h"
#include "UserWidgets/Inventory/Slots/InventorySlot.h"
#include "UserWidgets/Inventory/Slots/StorageSlot.h"

//================== Public ==================//
void UStoragePanel::SetItems(AStorageController* CurrentStorageController)
{
	if (StorageController == CurrentStorageController)
	{
		return;
	}
	ClearSlots();
	StorageController = CurrentStorageController;
	if (StorageController->StorageSpace > SlotArray.Num())
	{
		CreateSlots(StorageController->StorageSpace - SlotArray.Num());
	}
	FSlotDataArray* ResultEmptyArray = FindOrAdd("Empty");
	for (int x = 0; x< StorageController->StorageSpace;x++)
	{
		StorageSlots[x]->SetVisibility(ESlateVisibility::Visible);
		if (StorageController->InventoryItems.Num() > x
			&& StorageController->InventoryItems[x].Get<0>() != nullptr)
		{
			StorageSlots[x]->AddItem(StorageController->InventoryItems[x]);
			FindOrAdd(StorageController->InventoryItems[x].Get<0>()->GetItemName())->AddSlot(StorageSlots[x]);
			ResultEmptyArray->RemoveSlot(StorageSlots[x]);
		}
	}
}

FItemAddResult UStoragePanel::HandleAddItem(TTuple<FInventoryItems*, int32, int32> InventoryItems)
{
	if (InventoryItems.Get<0>()->IsStackable())
	{
		return HandleStackableItems(InventoryItems.Get<0>(), InventoryItems.Get<1>());
	}
	return HandleNonStackableItems(InventoryItems);
}

void UStoragePanel::RemoveSlot(USlotsHandler* ItemIn)
{
	FindOrAdd("Empty")->AddSlot(ItemIn);
	FindOrAdd(ItemIn->GetInventoryItems().Get<0>()->GetItemName())->RemoveSlot(ItemIn);
}

void UStoragePanel::AddSlot(USlotsHandler* ItemIn)
{
	FindOrAdd(ItemIn->GetInventoryItems().Get<0>()->GetItemName())->AddSlot(ItemIn);
}

//================== Protected ==================//
void UStoragePanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	InventoryPlayerController->SetStoragePanel(this);
}

void UStoragePanel::NativeConstruct()
{
	Super::NativeConstruct();

	SelectAll->OnPressed.AddDynamic(this, &UStoragePanel::HandleButtonClick);
	SelectWeapon->OnPressed.AddDynamic(this, &UStoragePanel::HandleButtonClick);
	SelectArmor->OnPressed.AddDynamic(this, &UStoragePanel::HandleButtonClick);
	SelectConsumable->OnPressed.AddDynamic(this, &UStoragePanel::HandleButtonClick);
	SelectAmmo->OnPressed.AddDynamic(this, &UStoragePanel::HandleButtonClick);
	SelectQuest->OnPressed.AddDynamic(this, &UStoragePanel::HandleButtonClick);
	SelectJunk->OnPressed.AddDynamic(this, &UStoragePanel::HandleButtonClick);
}

void UStoragePanel::CreateSlots(int32 SlotsCount)
{
	FSlotDataArray* ResultArray = FindOrAdd("Empty");

	for (int32 x = 0; x < SlotsCount; x++)
	{
		UStorageSlot* ItemSlot = CreateWidget<UStorageSlot>(this, StorageSlotClass);
		StorageSlots.Add(ItemSlot);
		ItemSlot->SetSlot(StorageSlots.Num() -1,this);
		ItemSlot->InventoryPlayerController = InventoryPlayerController;
		StoragePanel->AddChild(ItemSlot);
		ResultArray->AddSlot(ItemSlot);
	}
}

void UStoragePanel::ClearSlots()
{
	for (UStorageSlot* CurrentSlot : StorageSlots)
	{
		RemoveSlot(CurrentSlot);
		CurrentSlot->ClearSlot();
		CurrentSlot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UStoragePanel::HandleButtonClick()
{
	if (SelectAll->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EEmpty);
	}
	else if (SelectWeapon->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EWeapons);
	}
	else if (SelectArmor->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EArmor);
	}
	else if (SelectConsumable->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EConsumable);
	}
	else if (SelectAmmo->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EAmmo);
	}
	else if (SelectQuest->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EQuest);
	}
	else if (SelectJunk->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EJunk);
	}
}

int32 UStoragePanel::GetFreeSlotsCount()
{
	return FindOrAdd("Empty")->WidgetArray.Num();
}

FItemAddResult UStoragePanel::HandleNonStackableItems(TTuple<FInventoryItems*, int32, int32> InventoryItems)
{
	FSlotDataArray* ResultEmptyArray = FindOrAdd("Empty");
	if (ResultEmptyArray->GetSlotCount() <= 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UStoragePanel::HandleNonStackableItems If"));
		return FItemAddResult::AddNone(UMSGText::NoEmptySlot);
	}
	//UE_LOG(LogTemp, Warning, TEXT("UStoragePanel::HandleNonStackableItems after If"));
	FItemAddResult ItemAddResult = FItemAddResult::AddNone(FText::FromString("Can't Add Error."));

	ItemAddResult.SetupItem(InventoryItems.Get<0>()->GetItemName(), InventoryItems.Get<1>());

	for (int x = 0; x < ResultEmptyArray->WidgetArray.Num(); x++)
	{
		FindOrAdd(InventoryItems.Get<0>()->GetItemTypeAsName())->AddSlot(ResultEmptyArray->WidgetArray[x]);
		ResultEmptyArray->WidgetArray[x]->AddItem(MakeTuple(InventoryItems.Get<0>(), 1, InventoryItems.Get<2>()));
		ItemAddResult.RemoveCount(1);
		ResultEmptyArray->RemoveSlot(ResultEmptyArray->WidgetArray[x]);
		if (ItemAddResult.RemainingAmount == 0)
		{
			return ItemAddResult;
		}
		x--;
	}

	return ItemAddResult;
}

FItemAddResult UStoragePanel::HandleStackableItems(FInventoryItems* ItemIn, int32 InCount)
{
	//UE_LOG(LogTemp, Warning, TEXT("UStoragePanel::HandleStackableItems"));
	FItemAddResult ItemAddResult = FItemAddResult::AddNone(FText::FromString("Can't Add Error."));
	if (ItemIn->GetItemType() == EItemTypes::EAmmo)
	{
		ItemAddResult.SetupItem(ItemIn->GetAmmoItem()->GetActualItemName(), InCount);
	}
	else
	{
		ItemAddResult.SetupItem(ItemIn->GetItemName(), InCount);
	}

	if (FSlotDataArray* ResultArray = FindOrAdd(ItemIn->GetItemName()); ResultArray->GetSlotCount() > 0)
	{
		switch (ItemAddResult = ResultArray->AddCountToSlot(&ItemAddResult); ItemAddResult.OperationResult)
		{
		case EItemAddResultType::EAllItemAdded:
			return ItemAddResult;
		}
	}
	return AddNewItem(ItemIn, &ItemAddResult);
}

FItemAddResult UStoragePanel::AddNewItem(FInventoryItems* ItemIn, FItemAddResult* AddResult)
{
	FSlotDataArray* ResultEmptyArray = FindOrAdd("Empty");
	if (AddResult->RemainingAmount <= 0 || ResultEmptyArray->GetSlotCount() <= 0)
	{
		return *AddResult;
	}

	for (int x = 0; x < ResultEmptyArray->WidgetArray.Num(); x++)
	{
		if (AddResult->RemainingAmount <= ItemIn->GetMaxStack())
		{
			ResultEmptyArray->WidgetArray[x]->AddItem(MakeTuple(ItemIn, AddResult->RemainingAmount, 0));
			AddResult->RemoveCount(AddResult->RemainingAmount);
		}
		else
		{
			AddResult->RemoveCount(ItemIn->GetMaxStack());
			ResultEmptyArray->WidgetArray[x]->AddItem(MakeTuple(ItemIn, ItemIn->GetMaxStack(), 0));
		}

		FindOrAdd(ItemIn->GetItemName())->AddSlot(ResultEmptyArray->WidgetArray[x]);
		ResultEmptyArray->RemoveSlot(ResultEmptyArray->WidgetArray[x]);

		if (AddResult->RemainingAmount == 0)
		{
			return *AddResult;
		}
		x--;
	}
	return *AddResult;
}

FSlotDataArray* UStoragePanel::FindOrAdd(FName ArrayName)
{
	for (FSlotDataArray* SlotItem : SlotArray)
	{
		if (SlotItem && SlotItem->DataName == ArrayName)
		{
			return SlotItem;
		}
	}
	FSlotDataArray* EmptyList = new FSlotDataArray;
	EmptyList->DataName = ArrayName;
	SlotArray.Add(EmptyList);
	return EmptyList;
}
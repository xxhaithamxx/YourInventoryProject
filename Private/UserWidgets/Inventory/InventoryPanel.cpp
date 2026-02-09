// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/InventoryPanel.h"

#include "Component/InventoryComponent.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "Support/MSGText.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "LAGInventory/DataBase/Structs/Items/AmmoItem.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "Settings/InventoryPlayerController.h"
#include "Structs/SlotDataArray.h"
#include "UserWidgets/Inventory/Slots/CharacterSlot.h"
#include "UserWidgets/Inventory/Slots/InventorySlot.h"

struct FInventoryItems;


//================== Public ==================//
FItemAddResult UInventoryPanel::HandleAddItem(TTuple<FInventoryItems*, int32, int32> InventoryItems)
{
	if (InventoryItems.Get<0>()->IsStackable())
	{
		return HandleStackableItems(InventoryItems.Get<0>(), InventoryItems.Get<1>());
	}
	return HandleNonStackableItems(InventoryItems);
}

void UInventoryPanel::CreateSlots(int32 SlotsCount)
{
	FSlotDataArray* ResultArray = FindOrAdd("Empty");

	for (int32 x = 0; x < SlotsCount; x++)
	{
		UInventorySlot* ItemSlot = CreateWidget<UInventorySlot>(this, InventorySlotClass);
		ItemSlot->SetSlot(x, this);
		ItemSlot->InventoryPlayerController = InventoryPlayerController;
		InventoryPanel->AddChild(ItemSlot);
		ResultArray->AddSlot(ItemSlot);
	}
}

void UInventoryPanel::RemoveSlot(USlotsHandler* ItemIn)
{
	FindOrAdd("Empty")->AddSlot(ItemIn);
	FindOrAdd(ItemIn->GetInventoryItems().Get<0>()->GetItemName())->RemoveSlot(ItemIn);
}

void UInventoryPanel::AddSlot(USlotsHandler* ItemIn)
{
	FindOrAdd("Empty")->RemoveSlot(ItemIn);
	FindOrAdd(ItemIn->GetInventoryItems().Get<0>()->GetItemName())->AddSlot(ItemIn);
}

int32 UInventoryPanel::FindOrAddAmmo(FName ArrayName)
{
	FSlotDataArray* Array = FindOrAdd(ArrayName);
	if (Array->WidgetArray.Num() <= 0)
	{
		return 0;
	}
	int32 Value = 0;
	for (USlotsHandler* SlotItem : Array->WidgetArray)
	{
		Value += SlotItem->GetInventoryItems().Get<1>();
	}
	return Value;
}

void UInventoryPanel::ChangeCurrency(const int32 Amount)
{
	CurrentCurrency += Amount;
	Currency->SetText((FText::FromString("Currency : {0}"), FText::AsNumber(CurrentCurrency)));
}

bool UInventoryPanel::CanBuy(int32 Amount)
{
	return CurrentCurrency >= Amount;
}

//================== Protected ==================//
void UInventoryPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	InventoryPlayerController->SetInventoryPanel(this);
}

void UInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();
	if (InventoryPlayerController->GetInventoryComponent())
	{
		SlotsCapacity = InventoryPlayerController->GetInventoryComponent()->GetSlotsCapacity();
		CreateSlots(SlotsCapacity);
		SetInfoText();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryPanel::NativeConstruct InventoryComponent Not Set"));
	}

	SelectAll->OnPressed.AddDynamic(this, &UInventoryPanel::HandleButtonClick);
	SelectWeapon->OnPressed.AddDynamic(this, &UInventoryPanel::HandleButtonClick);
	SelectArmor->OnPressed.AddDynamic(this, &UInventoryPanel::HandleButtonClick);
	SelectConsumable->OnPressed.AddDynamic(this, &UInventoryPanel::HandleButtonClick);
	SelectAmmo->OnPressed.AddDynamic(this, &UInventoryPanel::HandleButtonClick);
	SelectQuest->OnPressed.AddDynamic(this, &UInventoryPanel::HandleButtonClick);
	SelectJunk->OnPressed.AddDynamic(this, &UInventoryPanel::HandleButtonClick);
}

void UInventoryPanel::HandleButtonClick()
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryPanel::HandleButtonClick"));
	if (SelectAll->IsPressed())
	{
		SortItem.Broadcast(EItemTypes::EEmpty);
	}
	else if (SelectWeapon->IsPressed())
	{
		SortItem.Broadcast(EItemTypes::EWeapons);
	}
	else if (SelectArmor->IsPressed())
	{
		SortItem.Broadcast(EItemTypes::EArmor);
	}
	else if (SelectConsumable->IsPressed())
	{
		SortItem.Broadcast(EItemTypes::EConsumable);
	}
	else if (SelectAmmo->IsPressed())
	{
		SortItem.Broadcast(EItemTypes::EAmmo);
	}
	else if (SelectQuest->IsPressed())
	{
		SortItem.Broadcast(EItemTypes::EQuest);
	}
	else if (SelectJunk->IsPressed())
	{
		SortItem.Broadcast(EItemTypes::EJunk);
	}
}

int32 UInventoryPanel::GetFreeSlotsCount()
{
	return FindOrAdd("Empty")->WidgetArray.Num();
}

FItemAddResult UInventoryPanel::HandleNonStackableItems(TTuple<FInventoryItems*, int32, int32> InventoryItems)
{
	FSlotDataArray* ResultEmptyArray = FindOrAdd("Empty");
	if (ResultEmptyArray->GetSlotCount() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryPanel::HandleNonStackableItems If"));
		return FItemAddResult::AddNone(UMSGText::NoEmptySlot);
	}
	UE_LOG(LogTemp, Warning, TEXT("UInventoryPanel::HandleNonStackableItems after If"));
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

FItemAddResult UInventoryPanel::HandleStackableItems(FInventoryItems* ItemIn, int32 InCount)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryPanel::HandleStackableItems"));
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

FItemAddResult UInventoryPanel::AddNewItem(FInventoryItems* ItemIn, FItemAddResult* AddResult)
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

void UInventoryPanel::SetInfoText()
{
	CapacityInfo->SetText(FText::Format(FText::FromString("{0}/{1}"),
	                                    GetFreeSlotsCount(),
	                                    SlotsCapacity));
}

FSlotDataArray* UInventoryPanel::FindOrAdd(FName ArrayName)
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

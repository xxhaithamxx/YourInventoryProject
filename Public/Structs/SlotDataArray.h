#pragma once

#include "CoreMinimal.h"
#include "ItemAddResult.h"
#include "UserWidgets/Inventory/Slots/SlotsHandler.h"
#include "LAGInventory/DataBase/Structs/Items/AmmoItem.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "SlotDataArray.generated.h"

USTRUCT(BlueprintType)
struct LAGINVENTORY_API FSlotDataArray
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TArray<USlotsHandler*> WidgetArray;

	FName DataName;

	void AddSlot(USlotsHandler* Slot)
	{
		if (!WidgetArray.Contains(Slot))
		{
			WidgetArray.Add(Slot);
			WidgetArray.Sort([](const USlotsHandler& ip1, const USlotsHandler& ip2)
			{
				return ip1.GetSlotID() < ip2.GetSlotID();
			});
		}
	}

	void RemoveSlot(USlotsHandler* Slot)
	{
		if (WidgetArray.Contains(Slot))
		{
			WidgetArray.Remove(Slot);
		}
	}

	int32 GetSlotCount() const
	{
		return WidgetArray.Num();
	}

	FItemAddResult AddCountToSlot(FItemAddResult* Add)
	{
		for (USlotsHandler* CurrentSlot : WidgetArray)
		{
			if (CurrentSlot->GetRemainingCount() <= 0)
			{
				continue;
			}

			if (CurrentSlot->GetInventoryItems().Get<0>()->GetItemType() == EItemTypes::EAmmo)
			{
				UE_LOG(LogTemp, Warning, TEXT("FSlotDataArray::AddCountToSlot 1"));
				if (CurrentSlot->GetInventoryItems().Get<0>()->GetAmmoItem()->GetActualItemName() != Add->ItemName)
				{
					UE_LOG(LogTemp, Warning, TEXT("FSlotDataArray::AddCountToSlot 2"));
					continue;
				}
			}
			// Check if there's enough remaining count to add 50
			if (CurrentSlot->GetRemainingCount() >= Add->RemainingAmount)
			{
				CurrentSlot->ChangeStack(Add->RemainingAmount);
				Add->RemoveCount(Add->RemainingAmount);
				return *Add;
			}
			Add->RemoveCount(CurrentSlot->GetRemainingCount());
			CurrentSlot->ChangeStack(CurrentSlot->GetRemainingCount());
		}
		return *Add;
	}
};

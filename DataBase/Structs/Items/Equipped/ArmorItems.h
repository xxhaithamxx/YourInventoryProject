#pragma once
#include "Component/CharacterStatsComponent.h"
#include "LAGInventory/DataBase/Enums/EquipType.h"
#include "LAGInventory/DataBase/Enums/ItemTypes.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "LAGInventory/DataBase/Structs/Stats/ItemStats.h"
#include "Settings/InventoryPlayerController.h"
#include "Support/InventorySupport.h"
#include "UserWidgets/Inventory/CharacterPanel.h"
#include "UserWidgets/Inventory/InventoryPanel.h"
#include "UserWidgets/Inventory/Slots/CharacterSlot.h"
#include "ArmorItems.generated.h"

USTRUCT(BlueprintType)
struct FArmorItems : public FInventoryItems
{
	GENERATED_USTRUCT_BODY()

	FArmorItems()
	{
		ItemType = EItemTypes::EArmor;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MinLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipType EquipType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BuyPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemStats EquipStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemStats> Stats;

public:
	virtual FItemAddResult
	OnUse(USlotsHandler* SlotsHandler, TTuple<FInventoryItems*, int32, int32> InventoryItem) override
	{
		if (SlotsHandler->InventoryPlayerController->GetCharacterStatsComponent()->PlayerLevel() >= MinLevel )
		{
			UCharacterSlot* EquipeSlots = nullptr;
			switch (EquipType)
			{
			case EEquipType::EJacket:
				EquipeSlots = SlotsHandler->InventoryPlayerController->GetCharacterPanel()->Boots;
				break;
			case EEquipType::EPants:
				break;
			case EEquipType::EShoes:
				EquipeSlots = SlotsHandler->InventoryPlayerController->GetCharacterPanel()->Boots;
			case EEquipType::ETShirt:
				break;
			case EEquipType::EVest:
				break;
			case EEquipType::EGlasses:
				break;
			case EEquipType::EHat:
				break;
			}
		
			if (!EquipeSlots)
			{
				return Super::OnUse(SlotsHandler, InventoryItem);
			}
		
			if (EquipeSlots->IsItemValid())
			{
				UInventorySupport::Swap(SlotsHandler->InventoryPlayerController,EquipeSlots,SlotsHandler);
				//SlotsHandler->InventoryPlayerController->GetInventoryPanel()->HandleSwapItems(SlotsHandler, EquipeSlots);
				return Super::OnUse(SlotsHandler, InventoryItem);
			}
			return EquipeSlots->AddItem(InventoryItem);
		}
		return Super::OnUse(SlotsHandler,InventoryItem);
	}

	FORCEINLINE virtual EEquipType GetEquipType() const override { return EquipType; }

	virtual int32 GetMinLevel() const override{ return MinLevel; }

	virtual	FORCEINLINE int32 GetBuyPrice() const override { return BuyPrice; }

	virtual FName GetItemTypeAsName() const override { return UEnum::GetValueAsName(EquipType);}

	FORCEINLINE FItemStats GetItemState() const { return EquipStats; }
	FORCEINLINE TArray<FItemStats> GetStats() const { return Stats; }


	virtual FORCEINLINE FArmorItems* GetArmorItems() override { return this; }
};

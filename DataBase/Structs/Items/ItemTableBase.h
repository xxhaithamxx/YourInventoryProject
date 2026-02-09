#pragma once
#include "Engine/DataTable.h"
#include "LAGInventory/DataBase/Enums/EquipType.h"
#include "LAGInventory/DataBase/Enums/ItemTypes.h"
#include "LAGInventory/DataBase/Enums/Weapons/WeaponType.h"
#include "Support/MSGText.h"
#include "Structs/ItemAddResult.h"
#include "ItemTableBase.generated.h"

struct FSlotDataHandler;
struct FRangedWeaponItem;
struct FMeleeWeaponItems;
struct FAmmoItem;
class UCharacterStatsComponent;
struct FArmorItems;
struct FConsumableItems;
struct FQuestItems;
struct FJunkItems;
struct FInventoryItems;
class UTooltip;
class USlotsHandler;

USTRUCT(BlueprintType)
struct FItemTableBase : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemTypes ItemType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Description;

public:
	FORCEINLINE virtual EItemTypes GetItemType() { return ItemType; }
	FORCEINLINE UTexture2D* GetIcon() const { return Icon; }
	FORCEINLINE virtual FName GetItemName() const { return ItemName; }
	FORCEINLINE FName GetDescription() const { return Description; }

#pragma region Chield Getter
	virtual FORCEINLINE FInventoryItems* GetInventoryItems()
	{
		UE_LOG(LogTemp, Warning, TEXT("FItemTableBase::GetInventoryItems"));
		return nullptr;
	}
	
	virtual FORCEINLINE FJunkItems* GetJunkItems() { return nullptr; }
	virtual FORCEINLINE FQuestItems* GetQuestItems() { return nullptr; }
	virtual FORCEINLINE FConsumableItems* GetConsumableItems() { return nullptr; }
	virtual FORCEINLINE FArmorItems* GetArmorItems() { return nullptr; }
	virtual FORCEINLINE FAmmoItem* GetAmmoItem() { return nullptr; }
	virtual FORCEINLINE FMeleeWeaponItems* GetMeleeItem() { return nullptr; }
	virtual FORCEINLINE FRangedWeaponItem* GetRangedItem() { return nullptr; }
	
	virtual FORCEINLINE USkeletalMesh* GetItemMesh() const { return nullptr; }

	virtual FORCEINLINE EWeaponType GetWeaponType() const { return EWeaponType::ENon;}
	virtual FORCEINLINE EEquipType GetEquipType() const { return EEquipType::ENon;}
#pragma endregion
	
	virtual void ShowToolTip(UTooltip* ToolTip)
	{
	}

	virtual void HideToolTip(UTooltip* ToolTip)
	{
	}

	virtual FItemAddResult OnUse (USlotsHandler* SlotsHandler,TTuple<FInventoryItems*, int32, int32> InventoryItem)
	{
		return FItemAddResult::AddNone(UMSGText::CantUseItem);
	}
};

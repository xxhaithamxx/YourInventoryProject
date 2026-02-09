#pragma once
#include "Component/CharacterStatsComponent.h"
#include "LAGInventory/DataBase/Enums/ItemTypes.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "LAGInventory/DataBase/Structs/Stats/ItemStats.h"
#include "Settings/InventoryPlayerController.h"
#include "Support/InventorySupport.h"
#include "UserWidgets/Inventory/CharacterPanel.h"
#include "UserWidgets/Inventory/InventoryPanel.h"
#include "UserWidgets/Inventory/Slots/CharacterSlot.h"
#include "MeleeWeaponItems.generated.h"

class AMeleeWeapon;
enum class EEquipType : uint8;
enum class EWeaponType : uint8;
enum class EWeaponEquipType : uint8;
enum class EWeaponSocket : uint8;

USTRUCT(BlueprintType)
struct FMeleeWeaponItems : public FInventoryItems
{
	GENERATED_USTRUCT_BODY()

	FMeleeWeaponItems()
	{
		ItemType = EItemTypes::EWeapons;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MinLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipType EquipType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponType WeaponType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BuyPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemStats EquipStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemStats> Stats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponSocket WeaponSocket;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMeleeWeapon> WeaponClass;
	
	virtual FItemAddResult OnUse(USlotsHandler* SlotsHandler,TTuple<FInventoryItems*, int32, int32> InventoryItem) override
	{
		if (SlotsHandler->InventoryPlayerController->GetCharacterStatsComponent()->PlayerLevel() >= MinLevel )
		{
			if (SlotsHandler->InventoryPlayerController->GetCharacterPanel()->MeleeWeapon->IsItemValid())
			{
				UInventorySupport::Swap(SlotsHandler->InventoryPlayerController,SlotsHandler->InventoryPlayerController->GetCharacterPanel()->MeleeWeapon,SlotsHandler);
				//SlotsHandler->InventoryPlayerController->GetInventoryPanel()->HandleSwapItems(SlotsHandler,SlotsHandler->InventoryPlayerController->GetCharacterPanel()->MeleeWeapon);
				return Super::OnUse(SlotsHandler,InventoryItem);
			}
			return SlotsHandler->InventoryPlayerController->GetCharacterPanel()->MeleeWeapon->AddItem(InventoryItem);	
		}
		return Super::OnUse(SlotsHandler,InventoryItem);
	}
	virtual int32 GetMinLevel() const override{ return MinLevel; }
	FORCEINLINE EEquipType GetEquipType() const { return EquipType; }
	FORCEINLINE virtual EWeaponType GetWeaponType() const override{ return WeaponType; }
	FORCEINLINE float GetAttackDelay() const { return AttackDelay; }

	FORCEINLINE int32 GetBuyPrice() const { return BuyPrice; }
	FORCEINLINE virtual int32 GetMaxStack() override { return 1; }

	FORCEINLINE FItemStats GetItemState() const { return EquipStats; }
	FORCEINLINE TArray<FItemStats> GetStats() const { return Stats; }

	FORCEINLINE UAnimMontage* GetAnimMontage() const { return AnimMontage; }
	FORCEINLINE EWeaponSocket GetWeaponSocket() const { return WeaponSocket; }

	virtual FORCEINLINE FMeleeWeaponItems* GetMeleeItem() override { return this; }
};

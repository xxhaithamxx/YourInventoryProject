#pragma once
#include "Component/CharacterStatsComponent.h"
#include "LAGInventory/DataBase/Enums/ItemTypes.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "LAGInventory/DataBase/Structs/Stats/ItemStats.h"
#include "Support/InventorySupport.h"
#include "UserWidgets/Inventory/CharacterPanel.h"
#include "Settings/InventoryPlayerController.h"

#include "UserWidgets/Inventory/Slots/CharacterSlot.h"
#include "RangedWeaponItem.generated.h"

class ARangedWeapon;
enum class EEquipType : uint8;
enum class EWeaponType : uint8;
enum class EWeaponEquipType : uint8;
enum class EAmmoType : uint8;
enum class EWeaponSocket : uint8;

USTRUCT(BlueprintType)
struct FRangedWeaponItem : public FInventoryItems
{
	GENERATED_USTRUCT_BODY()

	FRangedWeaponItem()
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
	FItemStats EquipStats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemStats> Stats;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAmmoType AmmoType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 RPM;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MagSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BuyPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Accuracy;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Range;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ReloadDelay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackDelay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AnimMontage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EWeaponSocket WeaponSocket;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ARangedWeapon> WeaponClass;
	
	virtual int32 GetMinLevel() const override { return MinLevel; }

	virtual
	FORCEINLINE EEquipType GetEquipType() const override { return EquipType; }

	FORCEINLINE virtual EWeaponType GetWeaponType() const override { return WeaponType; }

	virtual
	FORCEINLINE int32 GetBuyPrice() const override { return BuyPrice; }

	FORCEINLINE virtual int32 GetMaxStack() override { return 1; }
	FORCEINLINE FName GetAmmoName() const { return UEnum::GetValueAsName(AmmoType); }
	FORCEINLINE FItemStats GetItemState() const { return EquipStats; }
	FORCEINLINE TArray<FItemStats> GetStats() const { return Stats; }

	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }

	FORCEINLINE int32 GetRPM() const { return RPM; }
	FORCEINLINE float GetAccuracy() const { return Accuracy; }
	FORCEINLINE float GetRange() const { return Range; }
	FORCEINLINE float GetReloadDelay() const { return ReloadDelay; }
	FORCEINLINE float GetAttackDelay() const { return AttackDelay; }

	FORCEINLINE int32 GetMagSize() const { return MagSize; }
	FORCEINLINE UAnimMontage* GetAnimMontage() const { return AnimMontage; }
	FORCEINLINE EWeaponSocket GetWeaponSocket() const { return WeaponSocket; }

	virtual FORCEINLINE FRangedWeaponItem* GetRangedItem() override { return this; }

	virtual FItemAddResult
	OnUse(USlotsHandler* SlotsHandler, TTuple<FInventoryItems*, int32, int32> InventoryItem) override
	{
		if (SlotsHandler->InventoryPlayerController->GetCharacterStatsComponent()->PlayerLevel() >= MinLevel)
		{
			UCharacterSlot* EquipeSlots = nullptr;
			switch (WeaponType)
			{
			case EWeaponType::EPistol:
				EquipeSlots = SlotsHandler->InventoryPlayerController->GetCharacterPanel()->PrimaryWeapon;
				break;
			case EWeaponType::ERifle:
			case EWeaponType::EShotgun:
			case EWeaponType::EMachineGun:
			case EWeaponType::ESubMachineGun:
			case EWeaponType::ESniper:
				EquipeSlots = SlotsHandler->InventoryPlayerController->GetCharacterPanel()->SecondaryWeapon;
				break;
			}

			if (!EquipeSlots)
			{
				return Super::OnUse(SlotsHandler, InventoryItem);
			}

			if (EquipeSlots->IsItemValid())
			{
				UInventorySupport::Swap(SlotsHandler->InventoryPlayerController,EquipeSlots,SlotsHandler);
				return Super::OnUse(SlotsHandler, InventoryItem);
			}
			return EquipeSlots->AddItem(InventoryItem);
		}
		return Super::OnUse(SlotsHandler, InventoryItem);
	}
};

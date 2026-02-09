// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"


class ARangedWeapon;
class AMeleeWeapon;
class AEquipAble;
class UCharacterSlot;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquip, AEquipAble*, CurrentWeapon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnEquip, AEquipAble*, CurrentWeapon);

struct FArmorItems;
struct FRangedWeaponItem;
struct FMeleeWeaponItems;
struct FInventoryItems;
class AInventoryPlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LAGINVENTORY_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Sets default values for this component's properties
	UEquipmentComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void EquipWeapon(UCharacterSlot* CharacterSlot);

	void UnEquipWeapon(UCharacterSlot* CharacterSlot);

	void EquipArmor(UCharacterSlot* CharacterSlot);

	void UnEquipArmor(UCharacterSlot* CharacterSlot);

	TArray<FArmorItems*> ArmorItems;

public:
#pragma region Consumable
	FOnEquip OnEquip;
	FOnEquip OnUnEquip;
	FOnUnEquip OnRemoved;
#pragma endregion

	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

	void Equip(UCharacterSlot* CharacterSlot);

	void UnEquip(UCharacterSlot* CharacterSlot);

	void SwitchWeapon(int32 WeaponId);

	UPROPERTY()
	AEquipAble* CurrentWeapon;
	UPROPERTY()
	AMeleeWeapon* MeleeWeapon;
	UPROPERTY()
	ARangedWeapon* PrimaryWeapon;
	UPROPERTY()
	ARangedWeapon* SecondaryWeapon;
};

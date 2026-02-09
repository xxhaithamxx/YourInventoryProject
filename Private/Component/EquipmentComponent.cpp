// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/EquipmentComponent.h"

#include "Items/Equippable/MeleeWeapon.h"
#include "Items/Equippable/RangedWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "LAGInventory/DataBase/Structs/Items/Equipped/MeleeWeaponItems.h"
#include "LAGInventory/DataBase/Structs/Items/Equipped/RangedWeaponItem.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/Inventory/Slots/CharacterSlot.h"


// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	InventoryPlayerController->SetEquipmentComponent(this);
}

// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::Equip(UCharacterSlot* CharacterSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("UEquipmentComponent::Equip"));
	if (CharacterSlot->GetInventoryItems().Get<0>()->GetItemType() == EItemTypes::EWeapons)
	{
		EquipWeapon(CharacterSlot);
	}
	else
	{
		EquipArmor(CharacterSlot);
	}
}

void UEquipmentComponent::UnEquip(UCharacterSlot* CharacterSlot)
{
	if (CharacterSlot->GetInventoryItems().Get<0>()->GetItemType() == EItemTypes::EWeapons)
	{
		UnEquipWeapon(CharacterSlot);
	}
	else
	{
		UnEquipArmor(CharacterSlot);
	}
}

void UEquipmentComponent::SwitchWeapon(int32 WeaponId)
{
	switch (WeaponId)
	{  
		case 1:
			if (MeleeWeapon && CurrentWeapon != MeleeWeapon)
			{
				OnUnEquip.Broadcast(CurrentWeapon);
				CurrentWeapon = MeleeWeapon;
				OnEquip.Broadcast(CurrentWeapon);
			}
			break;
	case 2:
		if (PrimaryWeapon && CurrentWeapon != PrimaryWeapon)
		{
			OnUnEquip.Broadcast(CurrentWeapon);
			CurrentWeapon = PrimaryWeapon;
			OnEquip.Broadcast(CurrentWeapon);
		}
		break;
	case 3:
		if (SecondaryWeapon && CurrentWeapon != SecondaryWeapon)
		{
			OnUnEquip.Broadcast(CurrentWeapon);
			CurrentWeapon = SecondaryWeapon;
			OnEquip.Broadcast(CurrentWeapon);
		}
		break;
	}
}

void UEquipmentComponent::EquipWeapon(UCharacterSlot* CharacterSlot)
{
	AEquipAble* Temp = nullptr;

	switch (CharacterSlot->GetInventoryItems().Get<0>()->GetWeaponType())
	{
	case EWeaponType::EMelee:
		UE_LOG(LogTemp, Warning, TEXT("UEquipmentComponent::EquipWeapon EMelee"));
		Temp = MeleeWeapon = GetWorld()->SpawnActor<AMeleeWeapon>(
			CharacterSlot->GetInventoryItems().Get<0>()->GetMeleeItem()->WeaponClass);
		break;

	case EWeaponType::EPistol:
		Temp = PrimaryWeapon = GetWorld()->SpawnActor<ARangedWeapon>(
			CharacterSlot->GetInventoryItems().Get<0>()->GetRangedItem()->WeaponClass);
		break;

	case EWeaponType::ERifle:
	case EWeaponType::EShotgun:
	case EWeaponType::EMachineGun:
	case EWeaponType::ESubMachineGun:
	case EWeaponType::ESniper:
		UE_LOG(LogTemp, Warning, TEXT("UEquipmentComponent::EquipWeapon Others"));
		Temp = SecondaryWeapon = GetWorld()->SpawnActor<ARangedWeapon>(
			CharacterSlot->GetInventoryItems().Get<0>()->GetRangedItem()->WeaponClass);
		break;
	}

	if (Temp)
	{
		UE_LOG(LogTemp, Warning, TEXT("UEquipmentComponent::EquipWeapon Temp"));
		Temp->AddItem(CharacterSlot, this);
		if (!CurrentWeapon)
		{
			CurrentWeapon = Temp;
			OnEquip.Broadcast(CurrentWeapon);
		}
		else
		{
			OnUnEquip.Broadcast(Temp);
		}
	}
}

void UEquipmentComponent::UnEquipWeapon(UCharacterSlot* CharacterSlot)
{
	if (CurrentWeapon->CharacterSlot == CharacterSlot)
	{
		OnRemoved.Broadcast(CurrentWeapon);
		CurrentWeapon = nullptr;
	}
	else if (MeleeWeapon->CharacterSlot == CharacterSlot)
	{
		MeleeWeapon->Destroy();
		MeleeWeapon = nullptr;
	}
	else if (PrimaryWeapon->CharacterSlot == CharacterSlot)
	{
		PrimaryWeapon->Destroy();
		PrimaryWeapon = nullptr;
	}
	else if (SecondaryWeapon->CharacterSlot == CharacterSlot)
	{
		SecondaryWeapon->Destroy();
		SecondaryWeapon = nullptr;
	}
}

void UEquipmentComponent::EquipArmor(UCharacterSlot* CharacterSlot)
{
	ArmorItems.Add(CharacterSlot->GetInventoryItems().Get<0>()->GetArmorItems());
}

void UEquipmentComponent::UnEquipArmor(UCharacterSlot* CharacterSlot)
{
	if (ArmorItems.Contains(CharacterSlot->GetInventoryItems().Get<0>()->GetArmorItems()))
	{
		ArmorItems.Add(CharacterSlot->GetInventoryItems().Get<0>()->GetArmorItems());
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equippable/MeleeWeapon.h"

#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "LAGInventory/DataBase/Structs/Items/Equipped/MeleeWeaponItems.h"


// Sets default values
AMeleeWeapon::AMeleeWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeleeWeapon::AddItem(UCharacterSlot* CurrentCharacterSlot, UEquipmentComponent* CurrentEquipmentComponent)
{
	Super::AddItem(CurrentCharacterSlot, CurrentEquipmentComponent);

	CurrentWeapon = CharacterSlot->GetInventoryItems().Get<0>()->GetMeleeItem();
}

float AMeleeWeapon::Attack()
{
	return CurrentWeapon->GetAttackDelay();
}

UAnimMontage* AMeleeWeapon::GetAnimMontage()
{
	return CurrentWeapon->GetAnimMontage();
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Equippable/RangedWeapon.h"

#include "Engine/SkeletalMeshSocket.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "LAGInventory/DataBase/Structs/Items/Equipped/RangedWeaponItem.h"


// Sets default values
ARangedWeapon::ARangedWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARangedWeapon::AddItem(UCharacterSlot* CurrentCharacterSlot, UEquipmentComponent* CurrentEquipmentComponent)
{
	Super::AddItem(CurrentCharacterSlot, CurrentEquipmentComponent);

	CurrentWeapon = CharacterSlot->GetInventoryItems().Get<0>()->GetRangedItem();

	Socket = MeshComponent->GetSocketByName("AttackSocket");
}

float ARangedWeapon::Attack()
{
	return CurrentWeapon->GetAttackDelay();
}

UAnimMontage* ARangedWeapon::GetAnimMontage()
{
	return CurrentWeapon->GetAnimMontage();
}

FTransform ARangedWeapon::GetSocket()
{
	if (Socket)
	{
		return Socket->GetSocketTransform(MeshComponent);
	}
	return Super::GetSocket();
}

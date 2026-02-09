// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/InventoryCharacter.h"
#include "Component/CharacterStatsComponent.h"
#include "Component/EquipmentComponent.h"
#include "Component/InventoryComponent.h"

// Sets default values
AInventoryCharacter::AInventoryCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");
	CharacterStatsComponent= CreateDefaultSubobject<UCharacterStatsComponent>("CharacterStatsComponent");
	EquipmentComponent= CreateDefaultSubobject<UEquipmentComponent>("EquipmentComponent");
}

// Called when the game starts or when spawned
void AInventoryCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInventoryCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AInventoryCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AInventoryCharacter::Equip(FInventoryItems* SlotDataHandler)
{
	
}

void AInventoryCharacter::UnEquip(FInventoryItems* SlotDataHandler)
{
	
}


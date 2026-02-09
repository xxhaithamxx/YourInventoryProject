// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/CharacterPanel.h"

#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/Inventory/Slots/CharacterSlot.h"

void UCharacterPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	InventoryPlayerController->SetCharacterPanel( this);
}

void UCharacterPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

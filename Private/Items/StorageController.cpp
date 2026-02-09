// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/StorageController.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/Inventory/StoragePanel.h"

//================== Protected ==================//
// Sets default values
AStorageController::AStorageController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add("Item");
}


// Called every frame
void AStorageController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when the game starts or when spawned
void AStorageController::BeginPlay()
{
	Super::BeginPlay();
}

void AStorageController::UpdateInteractionWidget()
{
}

void AStorageController::BeginFocus()
{
	Super::BeginFocus();
	if (InventoryPlayerController)
	{
		InventoryPlayerController->OnInteract.AddDynamic(this, &AStorageController::Interact);
	}
}

void AStorageController::EndFocus()
{
	Super::EndFocus();
	if (InventoryPlayerController)
	{
		InventoryPlayerController->OnInteract.RemoveDynamic(this, &AStorageController::Interact);
	}
}

//================== Public ==================//

void AStorageController::Interact()
{
	InventoryPlayerController->GetStoragePanel()->SetItems(this);
	UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::Interact 1"));
	InventoryPlayerController->ChangeLeftWidgetSwitcher(1);
	InventoryPlayerController->ToggleInGameWidget();
}

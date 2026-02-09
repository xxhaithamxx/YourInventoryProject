// Fill out your copyright notice in the Description page of Project Settings.


#include "../../Public/Items/PickupItem.h"
#include "Settings/InventoryPlayerController.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "UserWidgets/InGameWidget.h"
#include "UserWidgets/Inventory/InventoryPanel.h"

// Sets default values
APickupItem::APickupItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add("Item");
}

// Called when the game starts or when spawned
void APickupItem::BeginPlay()
{
	Super::BeginPlay();
	if (!InventoryData.IsNull())
	{
		InventoryItem = MakeTuple(InventoryData.GetRow<FInventoryItems>(TEXT("ContextString")), ItemQuantity, AmmoQuantity);
		if (InventoryItem.Get<0>())
		{
			MeshComponent->SetSkeletalMesh(InventoryItem.Get<0>()->GetItemMesh());
		}
	}
}

// Called every frame
void APickupItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Drop
void APickupItem::InitializeDrop(TTuple<FInventoryItems*, int32, int32> NewInventoryItem)
{
	InventoryItem = NewInventoryItem;
	MeshComponent->SetSkeletalMesh(InventoryItem.Get<0>()->GetItemMesh());
}
#pragma endregion

void APickupItem::Interact()
{
	if (!IsPendingKillPending())
	{
		if (InventoryPlayerController->GetInventoryComponent())
		{
			//Check if item will be deleted default method

			switch (const FItemAddResult AddResult = InventoryPlayerController->GetInventoryPanel()->
				HandleAddItem(InventoryItem); AddResult.OperationResult)
			{
			case EItemAddResultType::ENoItemAdded: // Notification No Item Added 
				break;
			case EItemAddResultType::EAllItemAdded:
				InventoryPlayerController->GetInGameWidget()->ShowNotifications(InventoryItem.Get<0>()->GetIcon(),AddResult.ResultMessage);
				Destroy();
				break;
			case EItemAddResultType::EPartialAmountItemAdded:
				InventoryPlayerController->GetInGameWidget()->ShowNotifications(InventoryItem.Get<0>()->GetIcon(),AddResult.ResultMessage);
				InventoryItem.Get<1>() = AddResult.RemainingAmount;
				UpdateInteractionWidget();
				break;
			}
		}
	}
}

void APickupItem::UpdateInteractionWidget()
{
}

void APickupItem::BeginFocus()
{
	Super::BeginFocus();
	if (InventoryPlayerController)
	{
		InventoryPlayerController->OnInteract.AddDynamic(this, &APickupItem::Interact);
	}
}

void APickupItem::EndFocus()
{
	Super::EndFocus();
	if (InventoryPlayerController)
	{
		InventoryPlayerController->OnInteract.RemoveDynamic(this, &APickupItem::Interact);
	}
}

bool APickupItem::IsItemValid() const
{
	if (!InventoryItem.Get<0>() || InventoryData.IsNull() || InventoryData.RowName.IsEqual("None"))
	{
		return false;
	}
	return true;
}

void APickupItem::SetupItem()
{
	if (!InventoryData.IsNull() && InventoryData.RowName != "None")
	{
		MeshComponent->SetSkeletalMesh(InventoryData.GetRow<FInventoryItems>(TEXT("ContextString"))->GetItemMesh());
	}
}

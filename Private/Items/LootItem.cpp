// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/LootItem.h"

#include "Blueprint/UserWidget.h"
#include "Component/InventoryComponent.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/Inventory/LootPanel.h"

// Sets default values
ALootItem::ALootItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Tags.Add("Item");
}

// Called when the game starts or when spawned
void ALootItem::BeginPlay()
{
	Super::BeginPlay();
	if (LootPanelClass)
	{
		LootPanel = CreateWidget<ULootPanel>(GetWorld(), LootPanelClass);
		LootPanel->AddToViewport(0);
		LootPanel->SetVisibility(ESlateVisibility::Collapsed);
		LootPanel->SelfLootItem = this;
	}
}

// Called every frame
void ALootItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALootItem::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex)
{
	Super::EndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	LootPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void ALootItem::LootedAllItem()
{
	InventoryPlayerController->GetInventoryComponent()->RemoveOverlapItem(this);
	InventoryPlayerController->RemoveUIActive(LootPanel);
	if (IsValid(this))
	{
		Destroy();
	}
}

void ALootItem::BeginFocus()
{
	Super::BeginFocus();
	if (InventoryPlayerController)
	{
		InventoryPlayerController->OnInteract.AddDynamic(this, &ALootItem::Interact);
	}
}

void ALootItem::EndFocus()
{
	Super::EndFocus();
	if (InventoryPlayerController)
	{
		InventoryPlayerController->OnInteract.RemoveDynamic(this, &ALootItem::Interact);
	}
}

void ALootItem::Interact()
{
	LootPanel->SetVisibility(ESlateVisibility::Visible);
	InventoryPlayerController->AddUIActive(LootPanel);
}

void ALootItem::SetLootItems(TArray<FRandomLootDrop> DropItems) const
{
	for (const FRandomLootDrop DropItem : DropItems)
	{
		if (DropItem.GetDropChance() >= 100)
		{
			LootPanel->CreateSlot(DropItem.InventoryData.GetRow<FInventoryItems>(TEXT("ContextString")),
			                      FMath::RandRange(DropItem.GetMinDrop(), DropItem.GetMaxDrop()));
		}
		else if (FMath::RandRange(0, 100) <= DropItem.GetDropChance())
		{
			LootPanel->CreateSlot(DropItem.InventoryData.GetRow<FInventoryItems>(TEXT("ContextString")),
			                      FMath::RandRange(DropItem.GetMinDrop(), DropItem.GetMaxDrop()));
		}
	}
}

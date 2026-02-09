// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/LootPanel.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Items/LootItem.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/InventoryGameWidget.h"
#include "UserWidgets/Inventory/InventoryPanel.h"
#include "UserWidgets/Inventory/Slots/LootSlot.h"

void ULootPanel::NativeConstruct()
{
	Super::NativeConstruct();
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if (CloseWidget)
	{
		CloseWidget->OnClicked.AddDynamic(this, &ULootPanel::OnCloseClicked);
		LootAllWidget->OnClicked.AddDynamic(this, &ULootPanel::ULootPanel::OnLootAllClicked);
	}
}

void ULootPanel::OnCloseClicked()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
}

void ULootPanel::OnLootAllClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("ULootPanel::OnLootAllClicked Not Valied"));
	if (!InventoryPlayerController->GetInventoryPanel())
	{
		UE_LOG(LogTemp, Warning, TEXT("ULootPanel::OnLootAllClicked Not Valied"));
	}
	for (int32 x = 0; x < SlotsDataHandler.Num(); x++)
	{
		switch (const FItemAddResult Result = InventoryPlayerController->GetInventoryPanel()->HandleAddItem(
			SlotsDataHandler[x]->GetInventoryItems()); Result.OperationResult)
		{
		case EItemAddResultType::ENoItemAdded:
			return;
		case EItemAddResultType::EPartialAmountItemAdded:
			SlotsDataHandler[x]->UpdateStackUI();
			break;
		case EItemAddResultType::EAllItemAdded:
			SlotsDataHandler[x]->ClearSlot();
			x--;
			break;
		}
	}
}

void ULootPanel::CreateSlot(FInventoryItems* InventoryItems, const int32 SlotsCount)
{
	if (SlotsCount <= 0)
	{
		return;
	}
	ULootSlot* ItemSlot = CreateWidget<ULootSlot>(this, LootSlotClass);
	ItemSlot->AddItem(MakeTuple(InventoryItems, SlotsCount,0));
	SlotsDataHandler.Add(ItemSlot);
	ItemSlot->LootPanel = this;
	LootPanel->AddChild(ItemSlot);
}

void ULootPanel::RemoveSlot(ULootSlot* SlotDataHandler)
{
	if (SlotsDataHandler.Contains(SlotDataHandler))
	{
		SlotsDataHandler.Remove(SlotDataHandler);
		if (SlotsDataHandler.Num() <= 0)
		{
			this->RemoveFromParent();
			SelfLootItem->LootedAllItem();
		}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "../../../../Public/UserWidgets/Inventory/Slots/InventorySlot.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/Image.h"
#include "Components/InvalidationBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "LAGInventory/DataBase/Structs/Items/AmmoItem.h"
#include "LAGInventory/DataBase/Structs/Items/ConsumableItems.h"
#include "LAGInventory/DataBase/Structs/Items/Equipped/RangedWeaponItem.h"
#include "UserWidgets/DragItemVisual.h"
#include "UserWidgets/ItemDragDropOperation.h"
#include "UserWidgets/SplitWidget.h"
#include "Settings/InventoryPlayerController.h"
#include "Support/InventorySupport.h"
#include "UserWidgets/Inventory/InventoryPanel.h"
#include "UserWidgets/TooltipCompare.h"

//================== Protected ==================//
void UInventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SlotType = ESlotTypes::EInventory;

	SupportedSlotTypes = {
		ESlotTypes::EInventory,
		ESlotTypes::ECharacter,
		ESlotTypes::EBank,
		ESlotTypes::ELoot,
		ESlotTypes::EVendor
	};
	HoverEffect->SetVisibility(ESlateVisibility::Collapsed);
}

#pragma region UI Handle
void UInventorySlot::UpdateStackUI()
{
	if (InventoryItems.Get<0>()->GetItemType() == EItemTypes::EWeapons)
	{
		if (InventoryItems.Get<0>()->GetWeaponType() != EWeaponType::EMelee)
		{
			ItemCount->SetVisibility(ESlateVisibility::Visible);
			AmmoText->SetVisibility(ESlateVisibility::Visible);
			AmmoSeparated->SetVisibility(ESlateVisibility::Visible);
			ItemCount->SetText(FText::AsNumber(InventoryItems.Get<2>()));
			AmmoText->SetText(FText::AsNumber(AmmoCount));
		}
		return;
	}
	if (InventoryItems.Get<1>() > 1)
	{
		ItemCount->SetVisibility(ESlateVisibility::Visible);
		ItemCount->SetText(FText::AsNumber(InventoryItems.Get<1>()));
		return;
	}
	ItemCount->SetVisibility(ESlateVisibility::Collapsed);
}
#pragma endregion

#pragma region Mouse Events

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (!IsItemValid())
	{
		return Reply.Unhandled();
	}
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && InMouseEvent.IsShiftDown() && InventoryItems.Get<1>()
		> 1)
	{
		// Left mouse button + Shift key was clicked
		// Add your logic here
		if (SplitWidgetClass)
		{
			USplitWidget* SplitWidget = CreateWidget<USplitWidget>(GetWorld(), SplitWidgetClass);
			SplitWidget->AddToViewport(10);
			SplitWidget->ChangeSlider(this, 1, InventoryItems.Get<1>());
		}
		UE_LOG(LogTemp, Warning, TEXT("UInventorySlot::NativeOnMouseButtonDown Left Shift And Mouse Clicked"));
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply.Unhandled();
}

FReply UInventorySlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
                                                      const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	if (DelayBar->GetVisibility() == ESlateVisibility::Visible || !IsItemValid())
	{
		return Reply.Unhandled();
	}
	const FItemAddResult Result = InventoryItems.Get<0>()->OnUse(this, InventoryItems);
	ChangeStack(-Result.ActualAmountAdded);
	return Reply.Unhandled();
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                          UDragDropOperation*& OutOperation)
{
	if (DragItemVisualClass)
	{
		UDragItemVisual* DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragItemVisual->Icon->SetBrushFromTexture(InventoryItems.Get<0>()->GetIcon());
		DragItemVisual->ItemQuantity->SetText(FText::AsNumber(InventoryItems.Get<1>()));

		UItemDragDropOperation* DropOperation = NewObject<UItemDragDropOperation>();
		DropOperation->SlotType = SlotType;
		DropOperation->SlotsHandler = this;
		DropOperation->DefaultDragVisual = DragItemVisual; // Tell Drop what visual are we using;
		//if drag failed change DragItemVisual to subObject
		DropOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DropOperation;
	}
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	//Same Slot
	if (this == ItemDragDrop->SlotsHandler)
	{
		return true;
	}
	//Can't Add From Item From This Slot.
	if (!SupportedSlotTypes.Contains(ItemDragDrop->SlotsHandler->GetSlotType()))
	{
		return false;
	}
	//Merge 
	if (IsItemValid() && InventoryItems.Get<0>()->GetItemName() == ItemDragDrop->SlotsHandler->GetInventoryItems()
	                                                                           .Get<0>()->GetItemName())
	{
		if (GetRemainingCount() > 0)
		{
			if (GetInventoryItems().Get<0>()->GetItemType() == EItemTypes::EAmmo)
			{
				UE_LOG(LogTemp, Warning, TEXT("FSlotDataArray::AddCountToSlot 1"));
				if (GetInventoryItems().Get<0>()->GetAmmoItem()->GetActualItemName() != ItemDragDrop->SlotsHandler->
					GetInventoryItems().Get<0>()->GetAmmoItem()->GetActualItemName())
				{
					goto jump;
				}
			}

			if (ItemDragDrop->IsSplit && GetRemainingCount() < ItemDragDrop->SplitCount)
			{
				ItemDragDrop->SlotsHandler->ChangeStack(-ItemDragDrop->SplitCount);
				ChangeStack(ItemDragDrop->SplitCount);
				return true;
			}
			ItemDragDrop->SlotsHandler->ChangeStack(-GetRemainingCount());
			ChangeStack(GetRemainingCount());
		}
		return true;
	}
jump:
	//Move Split Item
	if (!IsItemValid() && ItemDragDrop->IsSplit)
	{
		AddItem(ItemDragDrop->SlotsHandler->GetInventoryItems());
		ItemDragDrop->SlotsHandler->ChangeStack(-ItemDragDrop->SplitCount);
		InventoryPlayerController->GetInventoryPanel()->RemoveSlot(this);
		return true;
	}
	//Swap
	UInventorySupport::Swap(InventoryPlayerController,this,ItemDragDrop->SlotsHandler);
	return true;
}
#pragma endregion

#pragma region Keyboard Events
FReply UInventorySlot::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsItemValid() && IsFocusable() && InKeyEvent.GetKey() == EKeys::LeftShift && (InventoryItems.Get<0>()->
		GetItemType() == EItemTypes::EArmor ||
		InventoryItems.Get<0>()->GetItemType() == EItemTypes::EWeapons))
	{
		if (InventoryPlayerController->GetCharacterPanel()->Boots->IsItemValid())
		{
			Tooltip->ShowCompared();
			InventoryPlayerController->GetCharacterPanel()->Boots->GetInventoryItems().Get<0>()->ShowToolTip(
				Tooltip->CompareItem);
			InventoryItems.Get<0>()->ShowToolTip(Tooltip->ToolTip);
		}
	}
	else
	{
		Tooltip->ShowSingle();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UInventorySlot::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsFocusable() && InKeyEvent.GetKey() == EKeys::LeftShift)
	{
		Tooltip->ShowSingle();
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}
#pragma endregion

#pragma region Events
void UInventorySlot::UpdateEvents()
{
	switch (InventoryItems.Get<0>()->GetItemType())
	{
	case EItemTypes::EQuest:
		InventoryPlayerController->GetInventoryComponent()->OnAddQuest.Broadcast(*InventoryItems.Get<0>());
		break;
	case EItemTypes::EConsumable:
		switch (InventoryItems.Get<0>()->GetConsumableItems()->GetConsumableType())
		{
		case EConsumableType::EFood:
		case EConsumableType::EHealth:
			InventoryPlayerController->GetInventoryComponent()->OnHealthUsed.AddDynamic(
				this, &UInventorySlot::UseAbleTimer);
			break;
		case EConsumableType::EDrink:
		case EConsumableType::EMana:
			InventoryPlayerController->GetInventoryComponent()->OnManaUsed.AddDynamic(
				this, &UInventorySlot::UseAbleTimer);
			break;
		}
		break;
	case EItemTypes::EAmmo:
		InventoryPlayerController->GetInventoryComponent()->OnAmmoEvent.
		                           Broadcast(InventoryItems.Get<0>()->GetAmmoItem()->GetAmmoType(),
		                                     InventoryItems.Get<1>());
		break;
	case EItemTypes::EWeapons:
		if (InventoryItems.Get<0>()->GetWeaponType() != EWeaponType::EMelee)
		{
			InventoryPlayerController->GetInventoryComponent()->OnAmmoEvent.AddDynamic(
				this, &UInventorySlot::OnAmmoEvent);
			AmmoCount = InventoryPlayerController->GetInventoryPanel()->FindOrAddAmmo(
				InventoryItems.Get<0>()->GetRangedItem()->GetAmmoName());
		}
		break;
	}
}

void UInventorySlot::OnAmmoEvent(const EAmmoType& AmmoType, int32 Count)
{
	if (AmmoType == InventoryItems.Get<0>()->GetRangedItem()->GetAmmoType())
	{
		AmmoCount += Count;
		UpdateStackUI();
	}
}

void UInventorySlot::OnSortItem(const EItemTypes& SelectedItemTypes)
{
	if (SelectedItemTypes == EItemTypes::EEmpty)
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else if (!IsItemValid())
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (InventoryItems.Get<0>()->GetItemType() == SelectedItemTypes)
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UInventorySlot::UseAbleTimer(const EConsumableType& ConsumableType, float CurrentDelay, float MaxDelay)
{
	if (InventoryItems.Get<0>()->GetItemType() == EItemTypes::EConsumable && InventoryItems.Get<0>()->
		GetConsumableItems()->GetConsumableType() == ConsumableType)
	{
		DelayBar->SetVisibility(ESlateVisibility::Visible);
		DelayBar->SetPercent(1 - (CurrentDelay / MaxDelay));
		if (CurrentDelay / MaxDelay >= 1)
		{
			DelayBar->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
#pragma endregion

//================== Public ==================//

#pragma region Stack Handle
void UInventorySlot::ChangeStack(int32 Count)
{
	InventoryItems.Get<1>() += Count;
	if (InventoryItems.Get<1>() <= 0)
	{
		ClearSlot();
	}
	else
	{
		//UpdateEvents();
		UpdateStackUI();
	}
}
#pragma endregion

#pragma region Item Controller
FItemAddResult UInventorySlot::AddItem(TTuple<FInventoryItems*, int32, int32> InputItem)
{
	InventoryItems = InputItem;
	Icon->SetBrushFromTexture(InventoryItems.Get<0>()->GetIcon());
	ItemQuality->SetColorAndOpacity(InventoryItems.Get<0>()->GetQuality());
	EnableOrDisableUI(true);
	Tooltip = CreateWidget<UTooltipCompare>(this, TooltipCompareClass);
	InventoryItems.Get<0>()->ShowToolTip(Tooltip->ToolTip);
	SetToolTip(Tooltip);
	UpdateEvents();
	UpdateStackUI();
	return Super::AddItem(InputItem);
}

bool UInventorySlot::ClearSlot()
{
	InventoryPlayerController->GetInventoryPanel()->RemoveSlot(this);
	if (InventoryItems.Get<0>()->GetItemType() == EItemTypes::EAmmo)
	{
		InventoryPlayerController->GetInventoryComponent()->OnAmmoEvent.Broadcast(
			InventoryItems.Get<0>()->GetAmmoItem()->GetAmmoType(), -InventoryItems.Get<1>());
	}
	InventoryItems.Get<0>() = nullptr;
	DelayBar->SetVisibility(ESlateVisibility::Collapsed);
	AmmoText->SetVisibility(ESlateVisibility::Collapsed);
	AmmoSeparated->SetVisibility(ESlateVisibility::Collapsed);
	InventoryPlayerController->GetInventoryComponent()->OnHealthUsed.RemoveDynamic(this, &UInventorySlot::UseAbleTimer);
	InventoryPlayerController->GetInventoryComponent()->OnManaUsed.RemoveDynamic(this, &UInventorySlot::UseAbleTimer);
	InventoryPlayerController->GetInventoryComponent()->OnAmmoEvent.RemoveDynamic(this, &UInventorySlot::OnAmmoEvent);
	EnableOrDisableUI(false);
	SetToolTipText(FText::GetEmpty());
	return true;
}

#pragma endregion

bool UInventorySlot::IsItemValid() const
{
	return InventoryItems.Get<0>() != nullptr;
}

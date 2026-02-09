// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/Slots/StorageSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LAGInventory/DataBase/Enums/ItemTypes.h"
#include "LAGInventory/DataBase/Structs/Items/AmmoItem.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "Settings/InventoryPlayerController.h"
#include "Support/InventorySupport.h"
#include "UserWidgets/DragItemVisual.h"
#include "UserWidgets/ItemDragDropOperation.h"
#include "UserWidgets/SplitWidget.h"
#include "UserWidgets/TooltipCompare.h"
#include "UserWidgets/Inventory/CharacterPanel.h"
#include "UserWidgets/Inventory/InventoryPanel.h"
#include "UserWidgets/Inventory/StoragePanel.h"
#include "UserWidgets/Inventory/Slots/CharacterSlot.h"

//================== Protected ==================//
void UStorageSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SlotType = ESlotTypes::EBank;

	SupportedSlotTypes = {
		ESlotTypes::EInventory,
		ESlotTypes::EBank,
	};
	HoverEffect->SetVisibility(ESlateVisibility::Collapsed);
}

#pragma region UI Handle
void UStorageSlot::UpdateStackUI()
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

FReply UStorageSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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
		UE_LOG(LogTemp, Warning, TEXT("UStorageSlot::NativeOnMouseButtonDown Left Shift And Mouse Clicked"));
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply.Unhandled();
}

FReply UStorageSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
                                                      const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	if (!IsItemValid())
	{
		return Reply.Unhandled();
	}
	const FItemAddResult Result = InventoryPlayerController->GetInventoryPanel()->HandleAddItem(InventoryItems);
	ChangeStack(-Result.ActualAmountAdded);
	return Reply.Unhandled();
}

void UStorageSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
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

bool UStorageSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	//Same Slot
	UE_LOG(LogTemp, Warning, TEXT("UStorageSlot::NativeOnDrop"));
	if (this == ItemDragDrop->SlotsHandler)
	{
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("UStorageSlot::NativeOnDrop 1"));
	//Can't Add From Item From This Slot.
	if (!SupportedSlotTypes.Contains(ItemDragDrop->SlotsHandler->GetSlotType()))
	{
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("UStorageSlot::NativeOnDrop 3"));
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
		InventoryPlayerController->GetStoragePanel()->RemoveSlot(this);
		return true;
	}
	//Swap
	UInventorySupport::Swap(InventoryPlayerController,this,ItemDragDrop->SlotsHandler);
	//InventoryPlayerController->GetStoragePanel()->HandleSwapItems(ItemDragDrop->SlotsHandler, this);
	return true;
}
#pragma endregion

#pragma region Keyboard Events
FReply UStorageSlot::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
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

FReply UStorageSlot::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsFocusable() && InKeyEvent.GetKey() == EKeys::LeftShift)
	{
		Tooltip->ShowSingle();
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}
#pragma endregion

#pragma region Events
void UStorageSlot::OnSortItem(const EItemTypes& SelectedItemTypes)
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
#pragma endregion

//================== Public ==================//

void UStorageSlot::SetSlot(int32 Value, UStoragePanel* NewStoragePanel)
{
	SlotID = Value;
	StoragePanel = NewStoragePanel;
	StoragePanel->OnSortItem.AddDynamic(this,&UStorageSlot::OnSortItem);
}

#pragma region Stack Handle
void UStorageSlot::ChangeStack(int32 Count)
{
	InventoryItems.Get<1>() += Count;
	if (InventoryItems.Get<1>() <= 0)
	{
		ClearSlot();
	}
	else
	{
		UpdateStackUI();
	}
}

bool UStorageSlot::IsFull() const
{
	if (InventoryItems.Get<0>()->IsStackable())
	{
		return InventoryItems.Get<0>()->GetMaxStack() - InventoryItems.Get<1>() <= 0;
	}
	return true;
}

int32 UStorageSlot::GetRemainingCount() const
{
	if (InventoryItems.Get<0>()->IsStackable())
	{
		return InventoryItems.Get<0>()->GetMaxStack() - InventoryItems.Get<1>();
	}
	return 0;
}
#pragma endregion

#pragma region Item Controller
FItemAddResult UStorageSlot::AddItem(TTuple<FInventoryItems*, int32, int32> InputItem)
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

bool UStorageSlot::ClearSlot()
{
	InventoryPlayerController->GetStoragePanel()->RemoveSlot(this);
	InventoryItems.Get<0>() = nullptr;
	AmmoText->SetVisibility(ESlateVisibility::Collapsed);
	AmmoSeparated->SetVisibility(ESlateVisibility::Collapsed);
	EnableOrDisableUI(false);
	SetToolTipText(FText::GetEmpty());
	return true;
}

#pragma endregion

bool UStorageSlot::IsItemValid() const
{
	return InventoryItems.Get<0>() != nullptr;
}
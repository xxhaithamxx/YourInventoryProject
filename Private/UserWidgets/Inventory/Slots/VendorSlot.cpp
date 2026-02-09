// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/Slots/VendorSlot.h"

#include "Blueprint/DragDropOperation.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "Settings/InventoryPlayerController.h"
#include "Structs/VendorSellItems.h"
#include "UserWidgets/DragItemVisual.h"
#include "UserWidgets/ItemDragDropOperation.h"
#include "UserWidgets/TooltipCompare.h"
#include "UserWidgets/Inventory/CharacterPanel.h"
#include "UserWidgets/Inventory/InventoryPanel.h"
#include "UserWidgets/Inventory/VendorPanel.h"
#include "UserWidgets/Inventory/Slots/CharacterSlot.h"

class UItemDragDropOperation;

//================== Protected ==================//
void UVendorSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SlotType = ESlotTypes::EVendor;

	SupportedSlotTypes = {
		ESlotTypes::EInventory,
	};
	HoverEffect->SetVisibility(ESlateVisibility::Collapsed);
}

#pragma region UI Handle
void UVendorSlot::UpdateStackUI()
{
	ItemCount->SetVisibility(ESlateVisibility::Collapsed);
}
#pragma endregion

#pragma region Mouse Events

FReply UVendorSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (!IsItemValid())
	{
		return Reply.Unhandled();
	}
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton) && InMouseEvent.IsShiftDown())
	{
		// Left mouse button + Shift key was clicked
		// Add your logic here
		/*if (SplitWidgetClass)
		{
			USplitWidget* SplitWidget = CreateWidget<USplitWidget>(GetWorld(), SplitWidgetClass);
			SplitWidget->AddToViewport(10);
			SplitWidget->ChangeSlider(this, 1, InventoryItems.Get<1>());
		}
		UE_LOG(LogTemp, Warning, TEXT("UVendorSlot::NativeOnMouseButtonDown Left Shift And Mouse Clicked"));*/
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply.Unhandled();
}

FReply UVendorSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
                                                   const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	if (!IsItemValid())
	{
		return Reply.Unhandled();
	}
	if (InventoryPlayerController->GetInventoryPanel()->CanBuy(InventoryItems->GetInventoryItem()->GetBuyPrice()))
	{
		FItemAddResult ItemAddResult = InventoryPlayerController->GetInventoryPanel()->HandleAddItem(
			MakeTuple(InventoryItems->GetInventoryItem(), 1, 0));
		if (ItemAddResult.OperationResult == EItemAddResultType::EAllItemAdded)
		{
			InventoryPlayerController->GetInventoryPanel()->ChangeCurrency(
				-InventoryItems->GetInventoryItem()->GetBuyPrice());
		}
	}
	return Reply.Unhandled();
}

void UVendorSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                       UDragDropOperation*& OutOperation)
{
	if (DragItemVisualClass)
	{
		UDragItemVisual* DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragItemVisual->Icon->SetBrushFromTexture(InventoryItems->GetInventoryItem()->GetIcon());
		DragItemVisual->ItemQuantity->SetText(FText::AsNumber(1));

		UItemDragDropOperation* DropOperation = NewObject<UItemDragDropOperation>();
		DropOperation->SlotType = SlotType;
		DropOperation->SlotsHandler = this;
		DropOperation->DefaultDragVisual = DragItemVisual; // Tell Drop what visual are we using;
		//if drag failed change DragItemVisual to subObject
		DropOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DropOperation;
	}
}

bool UVendorSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                               UDragDropOperation* InOperation)
{
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	//Same Slot
	UE_LOG(LogTemp, Warning, TEXT("UVendorSlot::NativeOnDrop"));
	if (this == ItemDragDrop->SlotsHandler)
	{
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("UVendorSlot::NativeOnDrop 1"));
	//Can't Add From Item From This Slot.
	if (!SupportedSlotTypes.Contains(ItemDragDrop->SlotsHandler->GetSlotType()))
	{
		return false;
	}
	UE_LOG(LogTemp, Warning, TEXT("UVendorSlot::NativeOnDrop 3"));
	//Move Split Item
	/*if (!IsItemValid() && ItemDragDrop->IsSplit)
	{
		AddItem(ItemDragDrop->SlotsHandler->GetInventoryItems());
		ItemDragDrop->SlotsHandler->ChangeStack(-ItemDragDrop->SplitCount);
		InventoryPlayerController->GetStoragePanel()->RemoveSlot(this);
		return true;
	}*/
	if (ItemDragDrop->SlotsHandler->GetInventoryItems().Get<0>()->GetItemType() == EItemTypes::EArmor || ItemDragDrop->
		SlotsHandler->GetInventoryItems().Get<0>()->GetItemType() == EItemTypes::EWeapons)
	{
		InventoryPlayerController->GetInventoryPanel()->ChangeCurrency(
			ItemDragDrop->SlotsHandler->GetInventoryItems().Get<0>()->GetSellPrice());
	}
	else
	{
		if (ItemDragDrop->IsSplit)
		{
			InventoryPlayerController->GetInventoryPanel()->ChangeCurrency(
				ItemDragDrop->SlotsHandler->GetInventoryItems().Get<0>()->GetSellPrice() * ItemDragDrop->SplitCount);
			ItemDragDrop->SlotsHandler->ChangeStack(-ItemDragDrop->SplitCount);
			return true;
		}
		InventoryPlayerController->GetInventoryPanel()->ChangeCurrency(
			ItemDragDrop->SlotsHandler->GetInventoryItems().Get<0>()->GetSellPrice() * ItemDragDrop->SlotsHandler->
			GetInventoryItems().Get<1>());
	}
	ItemDragDrop->SlotsHandler->ClearSlot();
	/*if (DragManager.Instance.SlotType == SlotTypes.Inventory || DragManager.Instance.SlotType == SlotTypes.Character)
	{
		if (DragManager.Instance.GetSLot.Item.ItemType == CategoryType.Armor || DragManager.Instance.GetSLot.Item.ItemType == CategoryType.Weapons)
			InventoryManager.Instance.AddCurrency(DragManager.Instance.GetSLot.Item.SellPrice);
		else InventoryManager.Instance.AddCurrency(DragManager.Instance.GetSLot.Item.SellPrice * DragManager.Instance.GetSLot.StackOrDurability);
		InventorySupport.ClearSlot(DragManager.Instance.GetSLot);
	}
	DragManager.Instance.ClearMovingItem();*/
	return true;
}
#pragma endregion

#pragma region Keyboard Events
FReply UVendorSlot::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsItemValid() && IsFocusable() && InKeyEvent.GetKey() == EKeys::LeftShift && (InventoryItems->GetInventoryItem()
		->
		GetItemType() == EItemTypes::EArmor ||
		InventoryItems->GetInventoryItem()->GetItemType() == EItemTypes::EWeapons))
	{
		if (InventoryPlayerController->GetCharacterPanel()->Boots->IsItemValid())
		{
			Tooltip->ShowCompared();
			InventoryPlayerController->GetCharacterPanel()->Boots->GetInventoryItems().Get<0>()->ShowToolTip(
				Tooltip->CompareItem);
			InventoryItems->GetInventoryItem()->ShowToolTip(Tooltip->ToolTip);
		}
	}
	else
	{
		Tooltip->ShowSingle();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

FReply UVendorSlot::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsFocusable() && InKeyEvent.GetKey() == EKeys::LeftShift)
	{
		Tooltip->ShowSingle();
	}
	return Super::NativeOnKeyUp(InGeometry, InKeyEvent);
}
#pragma endregion

#pragma region Events
void UVendorSlot::OnSortItem(const EItemTypes& SelectedItemTypes)
{
	if (SelectedItemTypes == EItemTypes::EEmpty)
	{
		this->SetVisibility(ESlateVisibility::Visible);
	}
	else if (!IsItemValid())
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (InventoryItems->GetInventoryItem()->GetItemType() == SelectedItemTypes)
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

void UVendorSlot::SetSlot(int32 Value, TObjectPtr<UVendorPanel> NewVendorPanel)
{
	SlotID = Value;
	VendorPanel = NewVendorPanel;
	VendorPanel->OnSortItem.AddDynamic(this, &UVendorSlot::OnSortItem);
}

#pragma region Item Controller
void UVendorSlot::ChangeStack(int32 Count)
{
	Super::ChangeStack(Count);
}

FItemAddResult UVendorSlot::AddItem(FVendorSellItems* VendorItem)
{
	if (VendorItem->GetInventoryItem() == nullptr)
	{
		return FItemAddResult::AddNone(UMSGText::AddError);
	}
	InventoryItems = VendorItem;
	Icon->SetBrushFromTexture(InventoryItems->GetInventoryItem()->GetIcon());
	ItemQuality->SetColorAndOpacity(InventoryItems->GetInventoryItem()->GetQuality());
	EnableOrDisableUI(true);
	Tooltip = CreateWidget<UTooltipCompare>(this, TooltipCompareClass);
	InventoryItems->GetInventoryItem()->ShowToolTip(Tooltip->ToolTip);
	SetToolTip(Tooltip);
	UpdateEvents();
	UpdateStackUI();
	return Super::AddItem(VendorItem);
}

bool UVendorSlot::ClearSlot()
{
	InventoryItems = nullptr;
	EnableOrDisableUI(false);
	SetToolTipText(FText::GetEmpty());
	return true;
}

#pragma endregion

bool UVendorSlot::IsItemValid() const
{
	return InventoryItems != nullptr && InventoryItems->GetInventoryItem() != nullptr;
}

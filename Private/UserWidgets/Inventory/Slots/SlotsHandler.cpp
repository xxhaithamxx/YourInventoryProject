// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/Slots/SlotsHandler.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Structs/VendorSellItems.h"
#include "Support/MSGText.h"
#include "UserWidgets/TooltipCompare.h"



//================== Protected ==================//

#pragma region  Native
void USlotsHandler::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void USlotsHandler::NativeConstruct()
{
	Super::NativeConstruct();
	SupportedSlotTypes = {
		ESlotTypes::EInventory,
		ESlotTypes::ECharacter,
		ESlotTypes::EBank,
		ESlotTypes::ELoot,
		ESlotTypes::EVendor
	};
}
#pragma endregion

void USlotsHandler::UpdateEvents()
{
}

#pragma region UI Handler
void USlotsHandler::EnableOrDisableUI(bool Stat)
{
	if (Stat)
	{
		Icon->SetVisibility(ESlateVisibility::Visible);
		UpdateStackUI();
	}
	else
	{
		Icon->SetVisibility(ESlateVisibility::Collapsed);
		ItemQuality->SetColorAndOpacity(DefaultColor);
		ItemCount->SetVisibility(ESlateVisibility::Collapsed);
		HoverEffect->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotsHandler::UpdateStackUI()
{
}
#pragma endregion

#pragma region Mouse Events
FReply USlotsHandler::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply USlotsHandler::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

void USlotsHandler::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (IsItemValid())
	{
		HoverEffect->SetVisibility(ESlateVisibility::Visible);
		Tooltip->SetVisibility(ESlateVisibility::Visible);
		SetIsFocusable(true);
		SetUserFocus(GetOwningPlayer());
	}
	else if (Tooltip)
	{
		Tooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotsHandler::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	HoverEffect->SetVisibility(ESlateVisibility::Collapsed);
	SetIsFocusable(false);
	if (Tooltip)
	{
		Tooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USlotsHandler::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
										 UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool USlotsHandler::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
								 UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
#pragma endregion

//================== Public ==================//

bool USlotsHandler::IsItemValid() const
{
	return false;
}

#pragma region Stack Handler
void USlotsHandler::ChangeStack(int32 Count)
{
}
#pragma endregion

#pragma region Item Controllers
FItemAddResult USlotsHandler::AddItem(TTuple<FInventoryItems*, int32, int32> InputItem)
{
	return FItemAddResult::AddNone(UMSGText::AddError);
}

FItemAddResult USlotsHandler::AddItem(FVendorSellItems* VendorItems)
{
	return FItemAddResult::AddNone(UMSGText::AddError);
}

bool USlotsHandler::ClearSlot()
{
	return false;
}

#pragma endregion

#pragma region Equip Controllers
FItemAddResult USlotsHandler::Equip()
{
	UE_LOG(LogTemp, Warning, TEXT("USlotsHandler::Equip"));
	return FItemAddResult::AddNone(UMSGText::AddError);
}

FItemAddResult USlotsHandler::UnEquip()
{
	UE_LOG(LogTemp, Warning, TEXT("USlotsHandler::UnEquip"));
	return FItemAddResult::AddNone(UMSGText::AddError);
}

#pragma endregion

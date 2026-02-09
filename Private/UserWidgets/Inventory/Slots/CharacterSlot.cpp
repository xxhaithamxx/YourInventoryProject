// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/Slots/CharacterSlot.h"

#include "Blueprint/DragDropOperation.h"
#include "Component/EquipmentComponent.h"
#include "Component/InventoryComponent.h"
#include "UserWidgets/TooltipCompare.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "LAGInventory/DataBase/Structs/Items/AmmoItem.h"
#include "LAGInventory/DataBase/Structs/Items/ConsumableItems.h"
#include "LAGInventory/DataBase/Structs/Items/Equipped/RangedWeaponItem.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/DragItemVisual.h"
#include "UserWidgets/ItemDragDropOperation.h"
#include "UserWidgets/Inventory/InventoryPanel.h"

//================== Protected ==================//
void UCharacterSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	SupportedSlotTypes = {
		ESlotTypes::EInventory,
		ESlotTypes::ECharacter,
		ESlotTypes::EBank,
	};
	HoverEffect->SetVisibility(ESlateVisibility::Collapsed);
	SlotType = ESlotTypes::ECharacter;
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

#pragma region UI Handle
void UCharacterSlot::UpdateStackUI()
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
	ItemCount->SetVisibility(ESlateVisibility::Collapsed);
}
#pragma endregion

#pragma region Mouse Events

FReply UCharacterSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (!IsItemValid())
	{
		return Reply.Unhandled();
	}
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply.Unhandled();
}

FReply UCharacterSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
                                                      const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	const FItemAddResult Result = InventoryPlayerController->GetInventoryPanel()->HandleAddItem(InventoryItems);
	if (Result.OperationResult == EItemAddResultType::EAllItemAdded)
	{
		ClearSlot();
	}
	return Reply.Unhandled();
}

void UCharacterSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                          UDragDropOperation*& OutOperation)
{
	if (DragItemVisualClass)
	{
		UDragItemVisual* DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragItemVisual->Icon->SetBrushFromTexture(InventoryItems.Get<0>()->GetIcon());
		DragItemVisual->ItemQuantity->SetText(FText::AsNumber(InventoryItems.Get<1>()));

		UItemDragDropOperation* DropOperation = NewObject<UItemDragDropOperation>();
		DropOperation->SlotsHandler = this;
		DropOperation->DefaultDragVisual = DragItemVisual; // Tell Drop what visual are we using;
		//if drag failed change DragItemVisual to subObject
		DropOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DropOperation;
	}
}

bool UCharacterSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                  UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("UCharacterSlot::NativeOnDrop"));
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	//Same Slot
	if (this == ItemDragDrop->SlotsHandler)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterSlot::NativeOnDrop 1"));
		return true;
	}
	//Can't Add From Item From This Slot.
	if (!SupportedSlotTypes.Contains(ItemDragDrop->SlotsHandler->GetSlotType()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterSlot::NativeOnDrop 2"));
		return false;
	}

	if (SupportedEquipTypes.Contains(ItemDragDrop->SlotsHandler->GetInventoryItems().Get<0>()->GetEquipType()))
	{
		UE_LOG(LogTemp, Warning, TEXT("UCharacterSlot::NativeOnDrop 3"));
		if (ItemDragDrop->SlotsHandler->GetInventoryItems().Get<0>()->GetEquipType() == EEquipType::EWeapon && !
			SupportedWeaponsTypes.Contains(ItemDragDrop->SlotsHandler->GetInventoryItems().Get<0>()->GetWeaponType()))
		{
			UE_LOG(LogTemp, Warning, TEXT("UCharacterSlot::NativeOnDrop 5"));
			return false;
		}
		if (IsItemValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("UCharacterSlot::NativeOnDrop 6"));
			//Swap
			UInventorySupport::Swap(InventoryPlayerController,this,ItemDragDrop->SlotsHandler);
			//InventoryPlayerController->GetInventoryPanel()->HandleSwapItems(ItemDragDrop->SlotsHandler, this);
			return false;
		}
		UE_LOG(LogTemp, Warning, TEXT("UCharacterSlot::NativeOnDrop 7"));
		AddItem(ItemDragDrop->SlotsHandler->GetInventoryItems());
		ItemDragDrop->SlotsHandler->ClearSlot();
	}

	return true;
}
#pragma endregion

#pragma region Events
void UCharacterSlot::OnAmmoEvent(const EAmmoType& AmmoType, int32 Count)
{
	if (AmmoType == InventoryItems.Get<0>()->GetRangedItem()->GetAmmoType())
	{
		AmmoCount += Count;
		UpdateStackUI();
	}
}
#pragma endregion

//================== Public ==================//

#pragma region Item Controller
FItemAddResult UCharacterSlot::AddItem(TTuple<FInventoryItems*, int32, int32> InputItem)
{
	InventoryItems = InputItem;
	Icon->SetBrushFromTexture(InventoryItems.Get<0>()->GetIcon());
	ItemQuality->SetColorAndOpacity(InventoryItems.Get<0>()->GetQuality());
	EnableOrDisableUI(true);
	if (InputItem.Get<0>()->GetItemType() == EItemTypes::EWeapons && InventoryItems.Get<0>()->GetWeaponType() !=
		EWeaponType::EMelee)
	{
		AmmoCount = InventoryPlayerController->GetInventoryPanel()->FindOrAddAmmo(
			InventoryItems.Get<0>()->GetRangedItem()->GetAmmoName());
		InventoryPlayerController->GetInventoryComponent()->OnAmmoEvent.AddDynamic(this, &UCharacterSlot::OnAmmoEvent);
	}
	UpdateStackUI();
	Tooltip = CreateWidget<UTooltipCompare>(this, TooltipCompareClass);
	InventoryItems.Get<0>()->ShowToolTip(Tooltip->ToolTip);
	SetToolTip(Tooltip);
	InventoryPlayerController->GetEquipmentComponent()->Equip(this);
	return FItemAddResult::AddAll(1, UMSGText::GetAddAll(1, InventoryItems.Get<0>()->GetItemName()));
}

bool UCharacterSlot::ClearSlot()
{
	InventoryPlayerController->GetEquipmentComponent()->UnEquip(this);
	InventoryItems.Get<0>() = nullptr;
	AmmoText->SetVisibility(ESlateVisibility::Collapsed);
	AmmoSeparated->SetVisibility(ESlateVisibility::Collapsed);
	InventoryPlayerController->GetInventoryComponent()->OnAmmoEvent.RemoveDynamic(this, &UCharacterSlot::OnAmmoEvent);
	EnableOrDisableUI(false);
	return true;
}

bool UCharacterSlot::IsItemValid() const
{
	return InventoryItems.Get<0>() != nullptr;
}

#pragma endregion

#pragma region Stack Handler
void UCharacterSlot::ChangeStack(const int32 Count)
{
	InventoryItems.Get<1>() += Count;
}
#pragma endregion

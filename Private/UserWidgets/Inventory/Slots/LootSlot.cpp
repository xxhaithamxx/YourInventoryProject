// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/Slots/LootSlot.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/TooltipCompare.h"
#include "UserWidgets/Inventory/InventoryPanel.h"
#include "UserWidgets/Inventory/LootPanel.h"

void ULootSlot::NativeConstruct()
{
	Super::NativeConstruct();
	SlotType = ESlotTypes::ELoot;
	SupportedSlotTypes = {
	};
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
}

#pragma region UI Handle
void ULootSlot::UpdateStackUI()
{
	if (InventoryItems.Get<1>() > 1)
	{
		ItemCount->SetVisibility(ESlateVisibility::Visible);
		ItemCount->SetText(FText::AsNumber(InventoryItems.Get<1>()));
		return;
	}
	ItemCount->SetVisibility(ESlateVisibility::Collapsed);
}
#pragma endregion

#pragma region Item Controller
FItemAddResult ULootSlot::AddItem(TTuple<FInventoryItems*, int32, int32> InputItem)
{
	InventoryItems = InputItem;
	Icon->SetBrushFromTexture(InventoryItems.Get<0>()->GetIcon());
	ItemQuality->SetColorAndOpacity(InventoryItems.Get<0>()->GetQuality());
	ItemName->SetText(FText::FromString(InventoryItems.Get<0>()->GetItemName().ToString()));
	ItemDescription->SetText(FText::FromString(InventoryItems.Get<0>()->GetItemName().ToString()));
	EnableOrDisableUI(true);
	Tooltip = CreateWidget<UTooltipCompare>(this, TooltipCompareClass);
	InventoryItems.Get<0>()->ShowToolTip(Tooltip->ToolTip);
	SetToolTip(Tooltip);
	UpdateEvents();
	return Super::AddItem(InventoryItems);
}

bool ULootSlot::ClearSlot()
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	LootPanel->RemoveSlot(this);
	return true;
}
#pragma endregion

#pragma region Stack Handler
void ULootSlot::ChangeStack(int32 Count)
{
	Super::ChangeStack(Count);
	InventoryItems.Get<1>() -= Count;
}
#pragma endregion

#pragma region Mouse Events
FReply ULootSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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

FReply ULootSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,
                                                 const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	const FItemAddResult Result = InventoryPlayerController->GetInventoryPanel()->HandleAddItem(InventoryItems);
	switch (Result.OperationResult)
	{
	case EItemAddResultType::ENoItemAdded:
		break;
	case EItemAddResultType::EPartialAmountItemAdded:
		ChangeStack(Result.ActualAmountAdded);
		UpdateStackUI();
		break;
	case EItemAddResultType::EAllItemAdded:
		ClearSlot();
		break;
	}
	return Reply.Unhandled();
}

bool ULootSlot::IsItemValid() const
{
	return InventoryItems.Get<0>() != nullptr;
}

#pragma endregion

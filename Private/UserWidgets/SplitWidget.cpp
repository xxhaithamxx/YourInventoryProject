// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/SplitWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "UserWidgets/DragItemVisual.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/ItemDragDropOperation.h"
#include "UserWidgets/Inventory/Slots/InventorySlot.h"

void USplitWidget::ChangeSlider(UInventorySlot* InventorySlot, int Min, int Max)
{
	SelectedInventorySlot = InventorySlot;
	ApplyChangeSlider(Min, Max);
}

void USplitWidget::ChangeSlider(UVendorSlot* VendorSlot, int Min, int Max)
{
	SelectedVendorSlot = VendorSlot;
	ApplyChangeSlider(Min, Max);
}

void USplitWidget::ChangeSlider(UStorageSlot* StorageSlot, int Min, int Max)
{
	SelectedStorageSlot = StorageSlot;
	ApplyChangeSlider(Min, Max);
}

void USplitWidget::OnFloatValueChangedEvent(float Value)
{
	CurrentSize = static_cast<int32>(Value);
	EditableQuantity->SetText(FText::FromString(FString::Printf(TEXT("%d"), CurrentSize)));
}

void USplitWidget::OnEditableTextBoxChangedEvent(const FText& Text)
{
	if (!InputType.Contains(UKismetStringLibrary::GetCharacterArrayFromString(Text.ToString()).Last()))
	{
		EditableQuantity->SetText(FText::FromString(UKismetStringLibrary::LeftChop(Text.ToString(), 1)));
	}
	else
	{
		CurrentSize = UKismetMathLibrary::Clamp(FCString::Atoi(*Text.ToString()), 0, MaxSize);
		EditableQuantity->SetText(FText::AsNumber(CurrentSize));
		SliderQuantity->SetValue(MaxSize);
	}
}

void USplitWidget::ApplyChangeSlider(int Min, int Max)
{
	MaxSize = Max;
	SliderQuantity->SetMinValue(Min);
	SliderQuantity->SetMaxValue(Max);
	SliderQuantity->SetStepSize(1);

	MaxQuantity->SetText(FText::FromString(FString::Printf(TEXT("%d"), Max)));
	EditableQuantity->SetText(FText::FromString(FString::Printf(TEXT("%d"), 1)));

	EditableQuantity->OnTextChanged.AddDynamic(this, &USplitWidget::OnEditableTextBoxChangedEvent);
	SliderQuantity->OnValueChanged.AddDynamic(this, &USplitWidget::OnFloatValueChangedEvent);

	Button_Accept->OnClicked.AddDynamic(this, &USplitWidget::OnAcceptButtonClicked);
	Button_Cancel->OnClicked.AddDynamic(this, &USplitWidget::OnCancelButtonClicked);
}

void USplitWidget::OnAcceptButtonClicked()
{
	/*if (ClickedDraggableContainer)
	{
		UClickedDraggableContainer* SplitWidget = CreateWidget<UClickedDraggableContainer>(
			GetWorld(), ClickedDraggableContainer);
		SplitWidget->AddToViewport(10);
	}
	OnCancelButtonClicked();*/

	/*if (DragItemVisualClass)
	{
		UDragItemVisual* DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragItemVisual->Icon->SetBrushFromTexture(SelectedInventorySlot->GetSlotDataHandler()->GetItemIcon());
		DragItemVisual->ItemQuantity->SetText(FText::AsNumber(CurrentSize));

		UItemDragDropOperation* DropOperation = NewObject<UItemDragDropOperation>();
		DropOperation->SlotsHandler = SelectedInventorySlot;
		DropOperation->DefaultDragVisual = DragItemVisual; // Tell Drop what visual are we using;
		//if drag failed change DragItemVisual to subObject
		DropOperation->Pivot = EDragPivot::TopLeft;

		//OutOperation = DropOperation;
	}

	// Create a new draggable item
	UDragDropOperation* NewDragDropOp = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (NewDragDropOp)
	{
		// Set the image and text for the new draggable item
		NewDragDropOp->Payload = this;
		NewDragDropOp->DefaultDragVisual = this;
		NewDragDropOp->WidgetToDrag = this;

		// Customize the appearance of the draggable item (optional)
		// NewDragDropOp->Pivot = FVector2D(0.5f, 0.5f);

		// Set the split count information
		if (NewDragDropOp->Payload)
		{
			USplitWidget* SplitWidgetPayload = Cast<USplitWidget>(NewDragDropOp->Payload);
			if (SplitWidgetPayload)
			{
				SplitWidgetPayload->ItemCount = SplitCount;
			}
		}

		// Begin the drag-and-drop operation
		FReply::Handled().BeginDragDrop(NewDragDropOp);

		FEventReply ReplyResult = UWidgetBlueprintLibrary::DetectDragIfPressed(
			InMouseEvent, this, EKeys::LeftMouseButton);

		FReply::DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}*/
}

void USplitWidget::OnCancelButtonClicked()
{
	Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0))->RemoveUIActive(this);
	if (IsInViewport())
	{
		// Remove the widget from the viewport
		RemoveFromParent();
	}

	// Optionally, you can call the conditional BeginDestroy to clean up resources
	if (!IsValid(this))
	{
		ConditionalBeginDestroy();
	}
}

void USplitWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                        UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("USplitWidget::NativeOnDragDetected"));
	if (DragItemVisualClass)
	{
		UDragItemVisual* DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass);
		DragItemVisual->Icon->SetBrushFromTexture(SelectedInventorySlot->GetInventoryItems().Get<0>()->GetIcon());
		DragItemVisual->ItemQuantity->SetText(FText::AsNumber(SliderQuantity->GetValue()));

		UItemDragDropOperation* DropOperation = NewObject<UItemDragDropOperation>();
		DropOperation->SlotsHandler = SelectedInventorySlot;
		DropOperation->DefaultDragVisual = DragItemVisual; // Tell Drop what visual are we using;
		//if drag failed change DragItemVisual to subObject
		DropOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DropOperation;
	}
}

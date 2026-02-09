// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Slider.h"
#include "SplitWidget.generated.h"

class UStorageSlot;
class UClickedDraggableContainer;
class UDraggableButton;
class UEditableTextBox;
class UDragItemVisual;
class UButton;
class UVendorSlot;
class UInventorySlot;
class UTextBlock;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API USplitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Item Data | Drag")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	USlider* SliderQuantity;
	
	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UEditableTextBox* EditableQuantity;

	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UTextBlock* MaxQuantity;

	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UButton* Button_Accept;

	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UButton* Button_Cancel;
	
	void ChangeSlider(UInventorySlot* InventorySlot, int Min,int Max);

	void ChangeSlider(UVendorSlot* VendorSlot, int Min,int Max);

	void ChangeSlider(UStorageSlot* StorageSlot, int Min,int Max);
	
	UFUNCTION()
	void OnCancelButtonClicked();

	UPROPERTY()
	int CurrentSize = 1;
protected:
	
	UPROPERTY()
	UInventorySlot* SelectedInventorySlot;

	UPROPERTY()
	TArray<FString> InputType = {"0","1","2","3","4","5","6","7","8","9"};
	
	UPROPERTY()
	UVendorSlot* SelectedVendorSlot;

	UPROPERTY()
	UStorageSlot* SelectedStorageSlot;
	
	UPROPERTY()
	int MaxSize;
	
	UFUNCTION()
	void  OnFloatValueChangedEvent( float Value);

	UFUNCTION()
	void OnEditableTextBoxChangedEvent( const FText& Text);
	
	UFUNCTION()
	void ApplyChangeSlider(int Min,int Max);
	
	UFUNCTION()
	void OnAcceptButtonClicked();

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
								  UDragDropOperation*& OutOperation) override;
};

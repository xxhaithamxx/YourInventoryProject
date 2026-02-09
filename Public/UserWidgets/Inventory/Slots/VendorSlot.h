// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotsHandler.h"
#include "Blueprint/UserWidget.h"
#include "VendorSlot.generated.h"

struct FVendorSellItems;
class USplitWidget;
class UVendorPanel;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UVendorSlot : public USlotsHandler
{
	GENERATED_BODY()

	//================== Protected ==================// 
protected:
	int32 SlotID = 0;
	//		Item		   ,Stack ,Ammo  ,Inventory Ammo (Ammo Not Implemented) 
	FVendorSellItems* InventoryItems;
	UPROPERTY()
	int32 AmmoCount;
	virtual void NativeOnInitialized() override;

#pragma region UI Handler
	virtual void UpdateStackUI() override;
#pragma endregion

#pragma region Mouse Events

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply
	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	                                  UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;

#pragma endregion
	
#pragma region Keyboard Events
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
#pragma endregion

	UPROPERTY()
	TObjectPtr<UVendorPanel> VendorPanel;

#pragma region Events
	UFUNCTION()
	void OnSortItem(const EItemTypes& SelectedItemTypes);
#pragma endregion

#pragma region User Widgets Icons
	UPROPERTY(EditAnywhere, Category="Item Data | Count", meta=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, Category="Item Data | Count", meta=(BindWidget))
	UTextBlock* BuyPrice;
#pragma endregion

	//================== Public ==================//
public:
#pragma region  ID Data
	virtual int32 GetSlotID() const override { return SlotID; }
	virtual void SetSlot(int32 Value, TObjectPtr<UVendorPanel> NewVendorPanel);

#pragma endregion

	UPROPERTY(EditAnywhere, Category="Item Data | Split")
	TSubclassOf<USplitWidget> SplitWidgetClass;

#pragma region Stack Handler
	virtual void ChangeStack(int32 Count) override;
#pragma endregion

#pragma region Item Controllers
	virtual FItemAddResult AddItem(FVendorSellItems* VendorItem) override;

	virtual bool ClearSlot() override;
#pragma endregion

	virtual bool IsItemValid() const override;
	virtual FVendorSellItems* GetInventoryItem() { return InventoryItems; }
};

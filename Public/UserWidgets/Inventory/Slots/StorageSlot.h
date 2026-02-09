// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotsHandler.h"
#include "StorageSlot.generated.h"

class USplitWidget;
enum class EItemTypes : uint8;
class UStoragePanel;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UStorageSlot : public USlotsHandler
{
	GENERATED_BODY()
	//================== Protected ==================// 
protected:
	int32 SlotID = 0;
	//		Item		   ,Stack ,Ammo  ,Inventory Ammo (Ammo Not Implemented) 
	TTuple<FInventoryItems*, int32, int32> InventoryItems;
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
	UStoragePanel* StoragePanel;

#pragma region Events
	UFUNCTION()
	void OnSortItem(const EItemTypes& SelectedItemTypes);
#pragma endregion

#pragma region User Widgets Icons
	UPROPERTY(EditAnywhere, Category="Item Data | Count", meta=(BindWidget))
	UTextBlock* AmmoSeparated;
	UPROPERTY(EditAnywhere, Category="Item Data | Count", meta=(BindWidget))
	UTextBlock* AmmoText;
#pragma endregion
	//================== Public ==================//
public:
#pragma region  ID Data
	virtual int32 GetSlotID() const override { return SlotID; }
	virtual void SetSlot(int32 Value , UStoragePanel* NewStoragePanel);
	
#pragma endregion

	
	UPROPERTY(EditAnywhere, Category="Item Data | Split")
	TSubclassOf<USplitWidget> SplitWidgetClass;

#pragma region Stack Handler
	virtual void ChangeStack(int32 Count) override;

	FORCEINLINE virtual bool IsFull() const override;

	virtual int32 GetRemainingCount() const override;
#pragma endregion

#pragma region Item Controllers
	virtual FItemAddResult AddItem(TTuple<FInventoryItems*, int32, int32> InputItem) override;

	virtual bool ClearSlot() override;
#pragma endregion

	virtual bool IsItemValid() const override;
	virtual TTuple<FInventoryItems*, int32, int32> GetInventoryItems() override {return InventoryItems;}
};

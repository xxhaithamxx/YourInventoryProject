// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotsHandler.h"
#include "Blueprint/UserWidget.h"
#include "LootSlot.generated.h"

class ULootPanel;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API ULootSlot : public USlotsHandler
{
	GENERATED_BODY()

protected:
	TTuple<FInventoryItems*, int32, int32> InventoryItems;
	
#pragma region User Widgets Icons
	UPROPERTY(EditAnywhere, Category="Item Data | Count", meta=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(EditAnywhere, Category="Item Data | Count", meta=(BindWidget))
	UTextBlock* ItemDescription;
#pragma endregion

#pragma region Mouse Events
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply
	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
#pragma endregion

public:
	virtual bool IsItemValid() const override;
	
	virtual void NativeConstruct() override;
#pragma region UI Handler
	virtual void UpdateStackUI() override;
#pragma endregion

#pragma region Item Controllers
	virtual FItemAddResult AddItem(TTuple<FInventoryItems*, int32, int32> InputItem) override;

	virtual bool ClearSlot() override;
#pragma endregion

#pragma region Stack Handler
	virtual void ChangeStack(int32 Count) override;
#pragma endregion

	virtual TTuple<FInventoryItems*, int32, int32> GetInventoryItems() override {return InventoryItems;}
	
	UPROPERTY()
	ULootPanel* LootPanel;
};

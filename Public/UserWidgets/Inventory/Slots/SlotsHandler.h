// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/SlotTypes.h"
#include "Structs/ItemAddResult.h"
#include "SlotsHandler.generated.h"

#pragma region Classes

struct FVendorSellItems;
class UInvalidationBox;
class AInventoryPlayerController;
struct FInventoryItems;
class UTooltipCompare;
class UDragItemVisual;
class UTextBlock;
class UImage;
class UTooltip;

#pragma endregion

/**
 * 
 */
UCLASS()
class LAGINVENTORY_API USlotsHandler : public UUserWidget
{
	GENERATED_BODY()

	//================== Protected ==================// 
protected:
	UPROPERTY(EditAnywhere)
	FLinearColor DefaultColor;

	UPROPERTY(EditAnywhere)
	ESlotTypes SlotType;

	UPROPERTY()
	TArray<ESlotTypes> SupportedSlotTypes;

	UPROPERTY(EditAnywhere, Category="Item Data | Drag")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	
#pragma region Tooltip
	UPROPERTY()
	UTooltipCompare* Tooltip;

	UPROPERTY(EditAnywhere, Category="Item Data | Tooltip")
	TSubclassOf<UTooltipCompare> TooltipCompareClass;
#pragma endregion
	
#pragma region User Widgets 
	
	UPROPERTY(EditAnywhere, Category="Item Data | Icon", meta=(BindWidget))
	UImage* Icon;
	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UImage* HoverEffect;
	UPROPERTY(EditAnywhere, Category="Item Data | Count", meta=(BindWidget))
	UTextBlock* ItemCount;
	UPROPERTY(EditAnywhere, Category="Item Data | Quality", meta=(BindWidget))
	UImage* ItemQuality;
#pragma endregion

#pragma region  Native
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
#pragma endregion

#pragma region UI Handler
	virtual void EnableOrDisableUI(bool Stat);
	virtual void UpdateStackUI();
#pragma endregion

#pragma region Mouse Events
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply
	NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
									  UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
							  UDragDropOperation* InOperation) override;
#pragma endregion
	
	virtual void UpdateEvents();

	//================== Public ==================//
public:
	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;
	
#pragma region Stack Handler
	virtual void ChangeStack(int32 Count);
#pragma endregion

#pragma region Item Controllers
	virtual FItemAddResult AddItem(TTuple<FInventoryItems*, int32, int32> InputItem);
	virtual FItemAddResult AddItem(FVendorSellItems* VendorItems);
	
	virtual bool ClearSlot();

	FORCEINLINE virtual bool IsFull() const { return true; }
	FORCEINLINE virtual int32 GetRemainingCount() const { return 0; }
#pragma endregion

#pragma region Equip Controllers
	virtual FItemAddResult Equip();
	virtual FItemAddResult UnEquip();
#pragma endregion

#pragma region Getter
	FORCEINLINE virtual ESlotTypes GetSlotType() const { return SlotType; }
	FORCEINLINE UImage* GetIcon() const { return Icon; }
	FORCEINLINE UImage* GetQuality() const { return ItemQuality; }
#pragma endregion

#pragma region Supported Type
	FORCEINLINE virtual bool IsSupportedSlotType(const ESlotTypes Type) const
	{
		return SupportedSlotTypes.Contains(Type);
	}
#pragma endregion

	virtual bool IsItemValid() const;

	FORCEINLINE virtual TTuple<FInventoryItems*, int32, int32> GetInventoryItems()
	{
		return MakeTuple(nullptr, 0, 0);
	}

	//================== Getter & Setter ==================//
	FORCEINLINE virtual int32 GetSlotID() const { return 0; }
};

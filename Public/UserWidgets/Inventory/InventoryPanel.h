// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/ItemAddResult.h"
#include "InventoryPanel.generated.h"

class UTextBlock;
class UCharacterSlot;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventorySortItem, const EItemTypes&, ItemTypes);

class UButton;
class AInventoryPlayerController;
class UInventorySlot;
class USlotsHandler;
struct FSlotDataArray;
struct FInventoryItems;
class UStorageSlot;
class UWrapBox;

/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UInventoryPanel : public UUserWidget
{
	GENERATED_BODY()

	//================== Public ==================//
public:
	FItemAddResult HandleAddItem(TTuple<FInventoryItems*, int32, int32> InventoryItems);
	void CreateSlots(int32 SlotsCount);
	void RemoveSlot(USlotsHandler* ItemIn);
	void AddSlot(USlotsHandler* ItemIn);

	int32 FindOrAddAmmo(FName ArrayName);

	void ChangeCurrency(int32 Amount);
	bool CanBuy(int32 Amount);

	FInventorySortItem SortItem;

	//================== Protected ==================//
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventorySlot> InventorySlotClass;
	
	TArray<FSlotDataArray*> SlotArray;
	
	UPROPERTY()
	int32 CurrentCurrency = 0;

#pragma region Add Items Controller
	FItemAddResult HandleNonStackableItems(TTuple<FInventoryItems*, int32, int32> InventoryItems);
	FItemAddResult HandleStackableItems(FInventoryItems* ItemIn, int32 InCount);
	FItemAddResult AddNewItem(FInventoryItems* ItemIn, FItemAddResult* AddResult);
#pragma endregion
	
	FSlotDataArray* FindOrAdd(FName ArrayName);
	void SetInfoText();
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UPROPERTY()
	int32 SlotsCapacity;

	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

#pragma region User Widgets Icons
	UPROPERTY(meta=(BindWidget))
	UWrapBox* InventoryPanel;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* Currency;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CapacityInfo;
	
	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UButton* SelectAll;
	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UButton* SelectWeapon;
	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UButton* SelectArmor;
	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UButton* SelectConsumable;
	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UButton* SelectAmmo;
	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UButton* SelectQuest;
	UPROPERTY(EditAnywhere, Category="Item Data | Hover", meta=(BindWidget))
	UButton* SelectJunk;
#pragma endregion

	// Method to handle button click events
	UFUNCTION()
	void HandleButtonClick();

	UFUNCTION()
	int32 GetFreeSlotsCount();
};

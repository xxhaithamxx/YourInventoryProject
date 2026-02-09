// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Structs/ItemAddResult.h"
#include "StoragePanel.generated.h"

class AStorageController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStorageSortItem, const EItemTypes&, ItemTypes);

class UButton;
class AInventoryPlayerController;
class USlotsHandler;
struct FSlotDataArray;
struct FInventoryItems;
class UStorageSlot;
class UWrapBox;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UStoragePanel : public UUserWidget
{
	GENERATED_BODY()
	//================== Public ==================//
public:
	void SetItems(AStorageController* CurrentStorageController);
	FItemAddResult HandleAddItem(TTuple<FInventoryItems*, int32, int32> InventoryItems);
	void RemoveSlot(USlotsHandler* ItemIn);
	void AddSlot(USlotsHandler* ItemIn);

	FStorageSortItem OnSortItem;

	//================== Protected ==================//
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UStorageSlot> StorageSlotClass;

	TArray<UStorageSlot*> StorageSlots;
	TArray<FSlotDataArray*> SlotArray;

	UPROPERTY()
	AStorageController* StorageController;
	
	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

	void CreateSlots(int32 SlotsCount);
	void ClearSlots();

	FSlotDataArray* FindOrAdd(FName ArrayName);

	UFUNCTION()
	int32 GetFreeSlotsCount();

#pragma region User Widgets Icons
	UPROPERTY(meta=(BindWidget))
	UWrapBox* StoragePanel;

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

	UFUNCTION()
	void HandleButtonClick();

#pragma region Add Items Controller
	FItemAddResult HandleNonStackableItems(TTuple<FInventoryItems*, int32, int32> InventoryItems);
	FItemAddResult HandleStackableItems(FInventoryItems* ItemIn, int32 InCount);
	FItemAddResult AddNewItem(FInventoryItems* ItemIn, FItemAddResult* AddResult);
#pragma endregion
	
	UPROPERTY()
	int32 SlotsCapacity;
};

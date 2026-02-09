// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "Structs/ItemAddResult.h"
#include "VendorPanel.generated.h"

class AStorageController;
class USlotsHandler;
class UButton;
class AInventoryPlayerController;
struct FSlotDataArray;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVendorSortItem, const EItemTypes&, ItemTypes);

class AVendorController;
class UWrapBox;
class UVendorSlot;

/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UVendorPanel : public UUserWidget
{
	GENERATED_BODY()

	//================== Public ==================//
public:
	FItemAddResult HandleAddItem(TTuple<FInventoryItems*, int32, int32> InventoryItems);

	FVendorSortItem OnSortItem;
	void SetItems(TObjectPtr<AVendorController> CurrentVendorController);

	void CreateSlot(int32 SlotsCount);
	//================== Protected ==================//
protected:
	TArray<UVendorSlot*> SlotsDataHandler;

	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UVendorSlot> VendorSlotClass;

	UPROPERTY()
	TObjectPtr<AVendorController> VendorController;

	virtual void NativeOnInitialized() override;

	TArray<UVendorSlot*> VendorSlots;

	UPROPERTY()
	TObjectPtr<AStorageController> StorageController;

	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

	void ClearSlots();


#pragma region User Widgets Icons
	UPROPERTY(meta=(BindWidget))
	UWrapBox* VendorPanel;

	UPROPERTY( meta=(BindWidget))
	UButton* SelectAll;
	UPROPERTY( meta=(BindWidget))
	UButton* SelectWeapon;
	UPROPERTY( meta=(BindWidget))
	UButton* SelectArmor;
	UPROPERTY( meta=(BindWidget))
	UButton* SelectConsumable;
	UPROPERTY( meta=(BindWidget))
	UButton* SelectAmmo;
	UPROPERTY( meta=(BindWidget))
	UButton* SelectQuest;
	UPROPERTY( meta=(BindWidget))
	UButton* SelectJunk;
#pragma endregion

	UFUNCTION()
	void HandleButtonClick();

	UPROPERTY()
	int32 SlotsCapacity;
};

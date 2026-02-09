// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlotsHandler.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSlot.generated.h"

enum class EWeaponType : uint8;
enum class EAmmoType : uint8;
enum class EEquipType : uint8;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UCharacterSlot : public USlotsHandler
{
	GENERATED_BODY()

	//================== Protected ==================//
protected:
	UPROPERTY(EditAnywhere, Category="Item Data | Equipment")
	TArray<EEquipType> SupportedEquipTypes;

	UPROPERTY(EditAnywhere, Category="Item Data | Equipment")
	TArray<EWeaponType> SupportedWeaponsTypes;

	//		Item		   ,Stack ,Ammo  ,Inventory Ammo
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

#pragma region Events
	UFUNCTION()
	void OnAmmoEvent(const EAmmoType& AmmoType, int32 Count);
#pragma endregion

#pragma region User Widgets Icons
	UPROPERTY(EditAnywhere, Category="Item Data | Count", meta=(BindWidget))
	UTextBlock* AmmoSeparated;
	UPROPERTY(EditAnywhere, Category="Item Data | Count", meta=(BindWidget))
	UTextBlock* AmmoText;
#pragma endregion
	
	//================== Public ==================//
public:
#pragma region Stack Handler
	virtual void ChangeStack(int32 Count) override;
#pragma endregion

#pragma region Item Controllers
	virtual FItemAddResult AddItem(TTuple<FInventoryItems*, int32, int32>  InputItem) override;
	virtual bool ClearSlot() override;

#pragma endregion

	virtual bool IsItemValid() const override;
	virtual TTuple<FInventoryItems*, int32, int32> GetInventoryItems() override {return InventoryItems;}
};

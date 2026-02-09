// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "LootPanel.generated.h"

class AInventoryPlayerController;
class UButton;
class UScrollBox;
class ALootItem;
struct FSlotDataHandler;
class ULootSlot;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API ULootPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UScrollBox* LootPanel;

	UPROPERTY(meta=(BindWidget))
	UButton* LootAllWidget;

	UPROPERTY(meta=(BindWidget))
	UButton* CloseWidget;

	void CreateSlot(FInventoryItems* InventoryItems, int32 SlotsCount);

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULootSlot> LootSlotClass;

	void RemoveSlot(ULootSlot* SlotDataHandler);

	UPROPERTY()
	ALootItem* SelfLootItem;

protected:
	TArray<ULootSlot*> SlotsDataHandler;

	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

	UFUNCTION(Blueprintable)
	void OnCloseClicked();

	UFUNCTION(Blueprintable)
	void OnLootAllClicked();
};

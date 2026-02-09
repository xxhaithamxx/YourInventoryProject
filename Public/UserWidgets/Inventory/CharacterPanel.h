// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterPanel.generated.h"

class AInventoryPlayerController;
class UCharacterSlot;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UCharacterPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	/*
	UPROPERTY(EditAnywhere, Category="Item Data | Equip", meta=(BindWidget))
	UCharacterSlot* Shield;*/

	UPROPERTY(EditAnywhere, Category="Item Data | Equip", meta=(BindWidget))
	UCharacterSlot* PrimaryWeapon;
	UPROPERTY(EditAnywhere, Category="Item Data | Equip", meta=(BindWidget))
	UCharacterSlot* SecondaryWeapon;
	UPROPERTY(EditAnywhere, Category="Item Data | Equip", meta=(BindWidget))
	UCharacterSlot* MeleeWeapon;
	
	UPROPERTY(EditAnywhere, Category="Item Data | Equip", meta=(BindWidget))
	UCharacterSlot* Boots;
	
	UPROPERTY(EditAnywhere, Category="Item Data | Equip", meta=(BindWidget))
	UCharacterSlot* Hat;
	
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;
};




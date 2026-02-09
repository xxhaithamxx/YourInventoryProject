// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tooltip.generated.h"

class USlotsHandler;
class UBorder;
class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UTooltip : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UBorder* ItemQuality;

	UPROPERTY(meta=(BindWidget))
	UImage* Icon;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemName;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemType;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemDescription;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* UsageText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* MaxStackSize;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* SellPrice;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* AmmoType;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* RequireLevel;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* ConsumableUse;
	/*UPROPERTY(meta=(BindWidget))
	UTextBlock* StackSize;*/
	
#pragma region Weapon Data
	/*UPROPERTY(meta=(BindWidget))
	UTextBlock* DamageValue;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ArmorRating;*/
#pragma endregion 
};

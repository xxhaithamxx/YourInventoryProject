#pragma once
#include "InventoryItems.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LAGInventory/DataBase/Enums/ItemTypes.h"
#include "UserWidgets/Tooltip.h"
#include "AmmoItem.generated.h"

enum class EAmmoType : uint8;

USTRUCT(BlueprintType)
struct FAmmoItem : public FInventoryItems
{
	GENERATED_USTRUCT_BODY()

	FAmmoItem()
	{
		ItemType = EItemTypes::EAmmo;
		MaxStack = 20;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStack = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EAmmoType AmmoType;

public:
	FORCEINLINE virtual bool IsStackable() override { return true; }
	FORCEINLINE virtual FName GetItemName() const override
	{
		UE_LOG(LogTemp, Warning, TEXT("FAmmoItem::GetItemName %s"), *UEnum::GetDisplayValueAsText(AmmoType).ToString());
		return UEnum::GetValueAsName(AmmoType);
	}

	FORCEINLINE virtual FName GetActualItemName() const { return ItemName; }

	FORCEINLINE virtual FAmmoItem* GetAmmoItem() override { return this; }
	FORCEINLINE virtual EAmmoType GetAmmoType() { return AmmoType; }
	FORCEINLINE virtual int32 GetMaxStack() override { return MaxStack; }

	virtual void ShowToolTip(UTooltip* ToolTip) override
	{
		ToolTip->ItemQuality->SetBrushColor(GetQuality());

		ToolTip->Icon->SetBrushFromTexture(Icon);

		ToolTip->ItemName->SetText(FText::FromString(ItemName.ToString()));
		ToolTip->ItemType->SetText(UEnum::GetDisplayValueAsText(ItemType));

		if (!Description.IsEqual("None"))
		{
			ToolTip->ItemDescription->SetVisibility(ESlateVisibility::Visible);
			ToolTip->ItemDescription->SetText(FText::FromString(Description.ToString()));
		}

		ToolTip->AmmoType->SetVisibility(ESlateVisibility::Visible);
		ToolTip->AmmoType->SetText(FText::Format(FText::FromString("Ammo Type: {0}"),
		                                         UEnum::GetDisplayValueAsText(AmmoType)));

		ToolTip->MaxStackSize->SetVisibility(ESlateVisibility::Visible);
		ToolTip->MaxStackSize->SetText(FText::Format(FText::FromString("Max Stack: {0}"), FText::AsNumber(MaxStack)));

		ToolTip->SellPrice->SetVisibility(ESlateVisibility::Visible);
		ToolTip->SellPrice->SetText(FText::Format(FText::FromString("Sell Price: {0}"), FText::AsNumber(SellPrice)));
	}
};

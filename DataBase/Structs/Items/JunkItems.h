#pragma once
#include "InventoryItems.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "LAGInventory/DataBase/Enums/ItemTypes.h"
#include "UserWidgets/Tooltip.h"
#include "JunkItems.generated.h"

USTRUCT(BlueprintType)
struct FJunkItems : public FInventoryItems
{
	GENERATED_USTRUCT_BODY()

	FJunkItems()
	{
		ItemType = EItemTypes::EJunk;
		MaxStack = 20;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStack = 20;

public:
	FORCEINLINE virtual int32 GetMaxStack() override { return MaxStack; }
	virtual FORCEINLINE FJunkItems* GetJunkItems() override { return this; }
	FORCEINLINE virtual bool IsStackable() override {return true;}
	virtual void ShowToolTip(UTooltip* ToolTip) override
	{
		ToolTip->ItemQuality->SetBrushColor(GetQuality());
		
		ToolTip->Icon->SetBrushFromTexture(Icon);
		
		ToolTip->ItemName->SetText(FText::FromString(GetItemName().ToString()));

		ToolTip->ItemType->SetText(UEnum::GetDisplayValueAsText(ItemType));

		if (!Description.IsEqual("None"))
		{
			ToolTip->ItemDescription->SetVisibility(ESlateVisibility::Visible);
			ToolTip->ItemDescription->SetText(FText::FromString(Description.ToString()));	
		}

		ToolTip->MaxStackSize->SetVisibility(ESlateVisibility::Visible);
		ToolTip->MaxStackSize->SetText(FText::Format(FText::FromString("Max Stack: {0}"), FText::AsNumber(MaxStack)));

		ToolTip->SellPrice->SetVisibility(ESlateVisibility::Visible);
		ToolTip->SellPrice->SetText(FText::Format(FText::FromString("Sell Price: {0}"), FText::AsNumber(SellPrice)));
	}
};

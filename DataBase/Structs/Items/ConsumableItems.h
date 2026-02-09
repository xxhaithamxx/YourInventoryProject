#pragma once

#include "InventoryItems.h"
#include "Component/CharacterStatsComponent.h"
#include "Component/InventoryComponent.h"
#include "LAGInventory/DataBase/Enums/ConsumableType.h"
#include "LAGInventory/DataBase/Enums/ItemTypes.h"
#include "Structs/ItemAddResult.h"
#include "Settings/InventoryPlayerController.h"
#include "ConsumableItems.generated.h"

USTRUCT(BlueprintType)
struct FConsumableItems : public FInventoryItems
{
	GENERATED_USTRUCT_BODY()

	FConsumableItems()
	{
		ItemType = EItemTypes::EConsumable;
		MaxStack = 20;
	}

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MinLevel = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EConsumableType ConsumableType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Restore;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CoolDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BuyPrice;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStack = 20;


#pragma region Sit able
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsSit = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SitTime;
#pragma endregion

#pragma region OverTime
	bool IsOverTime;

	float OverTime;
#pragma endregion

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* AnimMontage;

public:
	virtual
	FORCEINLINE int32 GetMinLevel() const override { return MinLevel; }

	FORCEINLINE EConsumableType GetConsumableType() const { return ConsumableType; }
	FORCEINLINE int32 GetRestore() const { return Restore; }
	FORCEINLINE float GetCoolDown() const { return CoolDown; }

	virtual
	FORCEINLINE int32 GetBuyPrice() const override { return BuyPrice; }

	FORCEINLINE virtual int32 GetMaxStack() override { return MaxStack; }

	FORCEINLINE bool GetIsSit() const { return IsSit; }
	FORCEINLINE float GetSitTime() const { return SitTime; }

	FORCEINLINE bool GetIsOverTime() const { return IsOverTime; }
	FORCEINLINE float GetOverTime() const { return OverTime; }

	FORCEINLINE UAnimMontage* GetAnimMontage() const { return AnimMontage; }

	virtual FORCEINLINE FConsumableItems* GetConsumableItems() override { return this; }
	FORCEINLINE virtual bool IsStackable() override { return true; }

	virtual FItemAddResult
	OnUse(USlotsHandler* SlotsHandler, TTuple<FInventoryItems*, int32, int32> InventoryItem) override
	{
		switch (ConsumableType)
		{
		case EConsumableType::EFood:
		case EConsumableType::EHealth:


			UE_LOG(LogTemp, Warning, TEXT("FConsumableItems::OnUse %hhd"),
			       SlotsHandler->InventoryPlayerController->GetCharacterStatsComponent()->CanUseHeal());
			if (SlotsHandler->InventoryPlayerController->GetCharacterStatsComponent()->CanUseHeal())
			{
				SlotsHandler->InventoryPlayerController->GetCharacterStatsComponent()->AddHeal(Restore);
				SlotsHandler->InventoryPlayerController->GetInventoryComponent()->HealthTimer(ConsumableType, CoolDown);
				return FItemAddResult::AddAll(1, UMSGText::GetUsedConsumableItems(Restore, ItemName, "Health"));
			}
			return FItemAddResult::AddNone(UMSGText::HealthFull);
		case EConsumableType::EDrink:
		case EConsumableType::EMana:
			if (SlotsHandler->InventoryPlayerController->GetCharacterStatsComponent()->CanUseMana())
			{
				SlotsHandler->InventoryPlayerController->GetCharacterStatsComponent()->AddMana(Restore);
				SlotsHandler->InventoryPlayerController->GetInventoryComponent()->HealthTimer(ConsumableType, CoolDown);
				return FItemAddResult::AddAll(1, UMSGText::GetUsedConsumableItems(Restore, ItemName, "Mana"));
			}
			return FItemAddResult::AddNone(UMSGText::HealthFull);
		}
		return FItemAddResult::AddNone(UMSGText::CantUseItem);
	}

	virtual void ShowToolTip(UTooltip* ToolTip) override
	{
		ToolTip->ItemQuality->SetBrushColor(GetQuality());

		ToolTip->Icon->SetBrushFromTexture(Icon);

		ToolTip->ItemName->SetText(FText::FromString(GetItemName().ToString()));
		ToolTip->ItemType->SetText(UEnum::GetDisplayValueAsText(ItemType));

		ToolTip->RequireLevel->SetVisibility(ESlateVisibility::Visible);
		ToolTip->RequireLevel->SetText(
			FText::Format(FText::FromString("Requires Level {0}"), FText::AsNumber(MinLevel)));

		if (!Description.IsEqual("None"))
		{
			ToolTip->ItemDescription->SetVisibility(ESlateVisibility::Visible);
			ToolTip->ItemDescription->SetText(FText::FromString(Description.ToString()));
		}

		ToolTip->MaxStackSize->SetVisibility(ESlateVisibility::Visible);
		ToolTip->MaxStackSize->SetText(FText::Format(FText::FromString("Max Stack: {0}"), FText::AsNumber(MaxStack)));

		ToolTip->SellPrice->SetVisibility(ESlateVisibility::Visible);
		ToolTip->SellPrice->SetText(FText::Format(FText::FromString("Sell Price: {0}"), FText::AsNumber(SellPrice)));


#pragma region Use Item UI

		// Construct the text string
		FString Text = FString::Printf(TEXT("Use: Restores %d "), Restore);

		// Append "Health" or "Mana" based on the consumable type
		if (ConsumableType == EConsumableType::EHealth || ConsumableType == EConsumableType::EFood)
		{
			Text += TEXT("Health");
		}
		else if (ConsumableType == EConsumableType::EMana || ConsumableType == EConsumableType::EDrink)
		{
			Text += TEXT("Mana");
		}

		// Append over time information
		Text += IsOverTime ? FString::Printf(TEXT(" over %.0f sec."), OverTime) : TEXT(".");

		// Append cooldown information
		Text += CoolDown >= 60
			        ? FString::Printf(TEXT(" %.0f Min Cooldown)."), CoolDown / 60)
			        : FString::Printf(TEXT(" %.0f Sec Cooldown)."), CoolDown);

		// Append "Must remain seated while eating" or "Must remain seated while drinking" based on IsSetable and consumable type
		if (IsSit)
		{
			Text += (ConsumableType == EConsumableType::EFood)
				        ? TEXT(" Must remain seated while eating.")
				        : TEXT(" Must remain seated while drinking.");
		}

		ToolTip->ConsumableUse->SetVisibility(ESlateVisibility::Visible);
		ToolTip->ConsumableUse->SetText(FText::FromString(Text));
#pragma endregion
	}
};

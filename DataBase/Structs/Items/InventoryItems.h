#pragma once
#include "ItemQuality.h"
#include "ItemTableBase.h"
#include "InventoryItems.generated.h"

class UMetaSoundSource;

USTRUCT(BlueprintType)
struct FInventoryItems : public FItemTableBase
{
	//public:
	GENERATED_USTRUCT_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FDataTableRowHandle Quality;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SellPrice = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMetaSoundSource* PickUpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMesh* ItemMesh;

	FLinearColor Color = FColor::Black;

public:
	FORCEINLINE FLinearColor GetQuality()
	{
		if (!Quality.IsNull() && Color == FColor::Black)
		{
			Color = Quality.GetRow<FItemQuality>(TEXT("ContextString"))->GetColor();
		}
		return Color;
	}

	FORCEINLINE float GetSellPrice() const { return SellPrice; }
	FORCEINLINE virtual int32 GetBuyPrice() const { return 0;}
	FORCEINLINE virtual FName GetItemTypeAsName() const { return ItemName;}
	FORCEINLINE UMetaSoundSource* GetPickUpSound() const { return PickUpSound; }
	FORCEINLINE virtual int32 GetMaxStack() { return 1; }
	FORCEINLINE virtual int32 GetMinLevel() const { return 1; }
	FORCEINLINE virtual FInventoryItems* GetInventoryItems() override { return this; }
	FORCEINLINE virtual USkeletalMesh* GetItemMesh() const override { return ItemMesh; }
	FORCEINLINE virtual bool IsStackable(){return false;}
};

#pragma once
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "VendorSellItems.generated.h"

USTRUCT(BlueprintType)
struct LAGINVENTORY_API FVendorSellItems
{
	GENERATED_USTRUCT_BODY()

public:
	FORCEINLINE bool GetIsLimited() const { return bIsLimited; }

	FORCEINLINE void SetCurrentSellCount(const int32 ReduceCurrent) { CurrentSellCount -= ReduceCurrent; }
	FORCEINLINE int32 GetCurrentSellCount() const { return CurrentSellCount; }
	FORCEINLINE int32 GetMaxSellCount() const { return MaxSellCount; }

	FORCEINLINE FInventoryItems* GetInventoryItem()
	{
		if (!InventoryData.IsNull() && InventoryData.RowName != "None")
		{
			return InventoryData.GetRow<FInventoryItems>(TEXT("ContextString"));
		}

		return nullptr;
	}

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	FDataTableRowHandle InventoryData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	bool bIsLimited = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	int32 CurrentSellCount = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	int32 MaxSellCount = 1;
};

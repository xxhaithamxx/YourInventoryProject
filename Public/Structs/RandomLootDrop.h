#pragma once

#include "RandomLootDrop.generated.h"

USTRUCT(BlueprintType)
struct LAGINVENTORY_API FRandomLootDrop
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	FDataTableRowHandle InventoryData;

	FORCEINLINE int32 GetMinDrop() const { return MinDrop; }

	FORCEINLINE int32 GetMaxDrop() const { return MaxDrop; }

	FORCEINLINE int32 GetDropChance() const { return DropChance; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	int32 MinDrop = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	int32 MaxDrop = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	int32 DropChance = 0;
};

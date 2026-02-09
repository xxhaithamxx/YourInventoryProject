#pragma once

#include "CoreMinimal.h"
#include "StorageDataArray.generated.h"

struct FInventoryItems;

USTRUCT(BlueprintType)
struct LAGINVENTORY_API FStorageDataArray
{
	GENERATED_USTRUCT_BODY()

public:
	int32 SlotID;
	TTuple<FInventoryItems*, int32, int32> InventoryItem;
};

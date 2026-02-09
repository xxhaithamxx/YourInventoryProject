#pragma once
#include "ItemStats.generated.h"


enum class EItemsStats : uint8;

USTRUCT(BlueprintType)
struct FItemStats
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemsStats Stats;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value;
};

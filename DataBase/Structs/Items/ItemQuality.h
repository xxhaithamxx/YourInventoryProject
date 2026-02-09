#pragma once

#include "Engine/DataTable.h"
#include "ItemQuality.generated.h"

USTRUCT(BlueprintType)
struct FItemQuality : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor Color;

public:
	FORCEINLINE FLinearColor GetColor() const { return Color; }
};

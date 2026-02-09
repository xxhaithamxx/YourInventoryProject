#pragma once
#include "CoreMinimal.h"
#include "ItemAddResultType.generated.h"

UENUM(BlueprintType)
enum class EItemAddResultType : uint8
{
	ENoItemAdded UMETA(DisplayName = "No Item Added"),
	EPartialAmountItemAdded UMETA(DisplayName = "Partial Amount Item Added"),
	EAllItemAdded UMETA(DisplayName = "All Item Added"),
};

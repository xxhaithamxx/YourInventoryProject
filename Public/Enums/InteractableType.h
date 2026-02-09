#pragma once
#include "CoreMinimal.h"
#include "InteractableType.generated.h"

UENUM(BlueprintType)
enum class EInteractableType : uint8
{
	EPickup,
	ECharacter,
	EDevice,
	EToggle,
	EContainer,
};

#pragma once
#include "SlotTypes.generated.h"

UENUM(BlueprintType)
 enum class ESlotTypes: uint8
{
	EInventory UMETA(DisplayName = "Inventory"),
	ECharacter UMETA(DisplayName = "Character"),
	EActionBar UMETA(DisplayName = "ActionBar"),
	EBank UMETA(DisplayName = "Bank"),
	EVendor UMETA(DisplayName = "Vendor"),
	ELoot UMETA(DisplayName = "Loot"),
	ESkills UMETA(DisplayName = "Skills"),
};

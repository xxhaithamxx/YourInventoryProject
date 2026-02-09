#pragma once

UENUM(BlueprintType)
enum class EItemsStats : uint8
{
	EHealth UMETA(DisplayName = "Health"),
	EDamage UMETA(DisplayName = "Damage"),
	EArmor UMETA(DisplayName = "Armor"),
};

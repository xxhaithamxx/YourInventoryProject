#pragma once

UENUM(BlueprintType)
enum class EConsumableType : uint8
{
	EFood UMETA(DisplayName = "Food"),
	EDrink UMETA(DisplayName = "Drink"),
	EHealth UMETA(DisplayName = "Health Potion"),
	EMana UMETA(DisplayName = "Mana Potion"),
};

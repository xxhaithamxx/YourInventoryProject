#pragma once

UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	EEmpty UMETA(DisplayName = "Empty"),
	EQuest UMETA(DisplayName = "Quest"),
	EJunk UMETA(DisplayName = "Junk"),
	EResource UMETA(DisplayName = "Resource"),
	EConsumable UMETA(DisplayName = "Consumable"),
	EArmor UMETA(DisplayName = "Armor") ,
	EWeapons UMETA(DisplayName = "Weapons") ,
	EAmmo UMETA(DisplayName = "Ammo") ,
	ESkill UMETA(DisplayName = "Skill") ,
};

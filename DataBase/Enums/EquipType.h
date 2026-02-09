#pragma once

UENUM(BlueprintType)
enum class EEquipType : uint8
{
	ENon UMETA(DisplayName = "Non"),
	EJacket UMETA(DisplayName = "Jacket"),
	EPants UMETA(DisplayName = "Pants"),
	EShoes UMETA(DisplayName = "Shoes"),
	ETShirt UMETA(DisplayName = "T-Shirt"),
	EVest UMETA(DisplayName = "Vest"),
	EGlasses UMETA(DisplayName = "Glasses"),
	EHat UMETA(DisplayName = "Hat"),
	EBackpack UMETA(DisplayName = "Backpack"),
	EWeapon UMETA(DisplayName = "Weapon"),
};

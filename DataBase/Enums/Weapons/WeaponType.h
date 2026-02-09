#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	ENon UMETA(DisplayName = "Non"),
	EMelee UMETA(DisplayName = "Melee"),
	EPistol UMETA(DisplayName = "Pistol"),
	ERifle UMETA(DisplayName = "Rifle"),
	EShotgun UMETA(DisplayName = "Shotgun"),
	EMachineGun UMETA(DisplayName = "MachineGun"),
	ESubMachineGun UMETA(DisplayName = "Sub MachineGun"),
	ESniper UMETA(DisplayName = "Sniper"),
};

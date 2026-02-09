#pragma once

UENUM(BlueprintType)
enum class ERangedAttackType : uint8
{
	ESingle UMETA(DisplayName = "Single"),
	ESemiAutomatic UMETA(DisplayName = "SemiAutomatic"),
	EAutomatic UMETA(DisplayName = "Automatic"),
};

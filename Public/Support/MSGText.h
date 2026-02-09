// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Internationalization/Text.h"
#include "MSGText.generated.h"

/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UMSGText : public UObject
{
	GENERATED_BODY()

public:
	inline static FText NoEmptySlot = FText::FromString(TEXT("No Empty Slot"));
	inline static FText AddError = FText::FromString("Can't Add Error.");
	inline static FText SwapAdded = FText::FromString("Items Swapped.");
	inline static FText CantUseItem = FText::FromString("This items can't be used.");
	inline static FText HealthFull = FText::FromString("Health is Full.");


	static FText GetAddAll(int32 Count, FName Name)
	{
		return FText::FromString(FString::Printf(TEXT("%d %s Added"), Count, *Name.ToString()));
	}

	static FText GetAddPartial(int32 Count, int32 CountAdded, FName Name)
	{
		return FText::FromString(FString::Printf(TEXT("%d from %d %s Added"),CountAdded, Count, *Name.ToString()));
	}

	static FText GetNoFreeSpace(FName Name)
	{
		return FText::FromString(FString::Printf(TEXT("Could not Add %s to inventory. No free space"), *Name.ToString()));
	}

	static FText GetUsedConsumableItems(int32 Count, FName Name, FName ItemType)
	{
		return FText::FromString("Used {0} to {1} {2}"), FText::FromString(Name.ToString()), FText::AsNumber(Count),
			FText::FromString(ItemType.ToString());
	}
};

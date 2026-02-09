#pragma once
#include "InteractionData.generated.h"

class IInteractableInterface;

USTRUCT(BlueprintType)
struct  FInteractionData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	float LastInteractionCheckTime;
	
	IInteractableInterface* CurrentInteractable;
};

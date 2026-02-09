#pragma once
#include "InteractableData.generated.h"

enum class EInteractableType : uint8;

USTRUCT(BlueprintType)
struct  FInteractableData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)
	EInteractableType Interactable;
	
	UPROPERTY(EditAnywhere)
	float InteractionDuration;
};

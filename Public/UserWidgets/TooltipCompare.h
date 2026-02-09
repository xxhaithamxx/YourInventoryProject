// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipCompare.generated.h"

/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UTooltipCompare : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta=(BindWidget))
	class UTooltip* ToolTip;
	UPROPERTY(meta=(BindWidget))
	UTooltip* CompareItem;

	void ShowSingle();
	void ShowCompared();
};

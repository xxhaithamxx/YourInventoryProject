// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/TooltipCompare.h"

#include "UserWidgets/Tooltip.h"

void UTooltipCompare::ShowSingle()
{
	CompareItem->SetVisibility(ESlateVisibility::Collapsed);
}

void UTooltipCompare::ShowCompared()
{
	CompareItem->SetVisibility(ESlateVisibility::Visible);
}

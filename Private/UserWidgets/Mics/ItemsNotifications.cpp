// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Mics/ItemsNotifications.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemsNotifications::SetNotifications(UTexture2D* Icon, FText Msg)
{
	ItemIcon->SetBrushFromTexture(Icon);
	ItemMsg->SetText(Msg);
}

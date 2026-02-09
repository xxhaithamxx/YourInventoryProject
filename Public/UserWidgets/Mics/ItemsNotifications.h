// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemsNotifications.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UItemsNotifications : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UImage* ItemIcon;
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ItemMsg;
	
	void SetNotifications(UTexture2D* Icon,FText Msg);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragItemVisual.generated.h"

class UTextBlock;
class UImage;
class UBorder;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UDragItemVisual : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="Item Data | Image", meta=(BindWidget))
	UImage* Icon;

	UPROPERTY(EditAnywhere, Category="Item Data | CountOrDelay", meta=(BindWidget))
	UTextBlock* ItemQuantity;

	UPROPERTY(EditAnywhere, Category="Item Data | CountOrDelay", meta=(BindWidget))
	UBorder* ItemBorder;
};

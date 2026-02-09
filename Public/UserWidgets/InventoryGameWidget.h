// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGameWidget.generated.h"

class UWidgetSwitcher;
class AInventoryPlayerController;
class UButton;
class UInventoryPanel;
class AInventoryCharacter;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UInventoryGameWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override; //run first
	virtual void NativeConstruct() override; // run after NativeOnInitialized 
	/*virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;*/

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseWidget;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* LeftWidgetSwitcher;
	
	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

	UFUNCTION()
	void OnButtonClicked();
public:
	void ChangeLeftWidgetSwitcher(int32 NewID);

};

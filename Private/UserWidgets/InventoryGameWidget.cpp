// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/InventoryGameWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Controller/InventoryCharacter.h"
#include "Items/PickupItem.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/ItemDragDropOperation.h"
#include "UserWidgets/Inventory/Slots/SlotsHandler.h"

void UInventoryGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//UE_LOG(LogTemp, Warning, TEXT("UInventoryGameWidget::NativeOnInitialized"));
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	InventoryPlayerController->SetInventoryGameWidget(this);
}

void UInventoryGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (CloseWidget)
	{
		CloseWidget->OnClicked.AddDynamic(this, &UInventoryGameWidget::OnButtonClicked);
	}
}

/*bool UInventoryGameWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                 UDragDropOperation* InOperation)
{
	if (const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation); ItemDragDrop->
		SlotsHandler)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.bNoFail = true;
		SpawnParameters.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		const FVector SpawnLocation{
			InventoryPlayerController->GetPlayerCharacter()->GetActorLocation() +
			InventoryPlayerController->GetPlayerCharacter()->GetActorForwardVector() * 150.f
		};
		const FTransform SpawnTransform{InventoryPlayerController->GetPlayerCharacter()->GetActorRotation(), SpawnLocation};

		APickupItem* Pickup = GetWorld()->SpawnActor<APickupItem>(
			APickupItem::StaticClass(), SpawnTransform, SpawnParameters);

		if (ItemDragDrop->IsSplit)
		{
			Pickup->InitializeDrop(ItemDragDrop->SlotsHandler->GetInventoryItems());
			ItemDragDrop->SlotsHandler->ChangeStack(-1 * ItemDragDrop->SplitCount);
			return true;
		}
		Pickup->InitializeDrop(ItemDragDrop->SlotsHandler->GetInventoryItems());
		return ItemDragDrop->SlotsHandler->ClearSlot();
	}
	return false;
}*/

FReply UInventoryGameWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (IsFocusable() && InKeyEvent.GetKey() == EKeys::I)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryGameWidget::NativeOnKeyDown"));
		InventoryPlayerController->ToggleInGameWidget();
		return  Super::NativeOnKeyDown(InGeometry, InKeyEvent).Handled();
	}
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UInventoryGameWidget::OnButtonClicked()
{
	InventoryPlayerController->ToggleInGameWidget();
}

void UInventoryGameWidget::ChangeLeftWidgetSwitcher(int32 NewID)
{
	LeftWidgetSwitcher->SetActiveWidgetIndex(NewID);
}


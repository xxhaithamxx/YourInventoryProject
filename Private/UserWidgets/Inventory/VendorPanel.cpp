// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/Inventory/VendorPanel.h"

#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "Controller/VendorController.h"
#include "Kismet/GameplayStatics.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"
#include "Settings/InventoryPlayerController.h"
#include "Structs/SlotDataArray.h"
#include "Structs/VendorSellItems.h"
#include "UserWidgets/Inventory/Slots/VendorSlot.h"

//================== Public ==================//
void UVendorPanel::SetItems(TObjectPtr<AVendorController> CurrentVendorController)
{
	if (VendorController == CurrentVendorController)
	{
		return;
	}
	ClearSlots();
	VendorController = CurrentVendorController;
	if (VendorController->VendorSellItems.Num() > VendorSlots.Num())
	{
		CreateSlot(VendorController->VendorSellItems.Num() - VendorSlots.Num());
	}

	// Use a traditional for loop
	for (int x = 0; x < VendorController->VendorSellItems.Num(); x++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UVendorPanel::SetItems for = %d"),x);
		VendorSlots[x]->SetVisibility(ESlateVisibility::Visible);
		VendorSlots[x]->AddItem(&VendorController->VendorSellItems[x]);
	}
}

void UVendorPanel::CreateSlot(int32 SlotsCount)
{
	for (int32 x = 0; x < SlotsCount; x++)
	{
		UVendorSlot* ItemSlot = CreateWidget<UVendorSlot>(this, VendorSlotClass);
		VendorSlots.Add(ItemSlot);
		ItemSlot->SetSlot(VendorSlots.Num() - 1, this);
		ItemSlot->InventoryPlayerController = InventoryPlayerController;
		VendorPanel->AddChild(ItemSlot);
	}
}


//================== Protected ==================//
void UVendorPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//UE_LOG(LogTemp, Warning, TEXT("UVendorPanel::NativeOnInitialized"));
	InventoryPlayerController->SetVendorPanel(this);
}

void UVendorPanel::NativeConstruct()
{
	Super::NativeConstruct();

	SelectAll->OnPressed.AddDynamic(this, &UVendorPanel::HandleButtonClick);
	SelectWeapon->OnPressed.AddDynamic(this, &UVendorPanel::HandleButtonClick);
	SelectArmor->OnPressed.AddDynamic(this, &UVendorPanel::HandleButtonClick);
	SelectConsumable->OnPressed.AddDynamic(this, &UVendorPanel::HandleButtonClick);
	SelectAmmo->OnPressed.AddDynamic(this, &UVendorPanel::HandleButtonClick);
	SelectQuest->OnPressed.AddDynamic(this, &UVendorPanel::HandleButtonClick);
	SelectJunk->OnPressed.AddDynamic(this, &UVendorPanel::HandleButtonClick);
}

void UVendorPanel::ClearSlots()
{
	//UE_LOG(LogTemp, Warning, TEXT("UVendorPanel::ClearSlots"));
	for (UVendorSlot* CurrentSlot : VendorSlots)
	{
	//	UE_LOG(LogTemp, Warning, TEXT("UVendorPanel::ClearSlots for"));
		CurrentSlot->ClearSlot();
		CurrentSlot->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UVendorPanel::HandleButtonClick()
{
	//UE_LOG(LogTemp, Warning, TEXT("UVendorPanel::HandleButtonClick"));
	if (SelectAll->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EEmpty);
	}
	else if (SelectWeapon->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EWeapons);
	}
	else if (SelectArmor->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EArmor);
	}
	else if (SelectConsumable->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EConsumable);
	}
	else if (SelectAmmo->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EAmmo);
	}
	else if (SelectQuest->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EQuest);
	}
	else if (SelectJunk->IsPressed())
	{
		OnSortItem.Broadcast(EItemTypes::EJunk);
	}
}

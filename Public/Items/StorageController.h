// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemsBase.h"
#include "GameFramework/Actor.h"
#include "StorageController.generated.h"

struct FStorageDataArray;
struct FInventoryItems;
struct FSlotDataArray;

UCLASS()
class LAGINVENTORY_API AStorageController : public AItemsBase
{
	GENERATED_BODY()

	//================== Public ==================//

public:

	virtual void Interact() override;

	UPROPERTY(EditAnywhere)
	int32 StorageSpace = 10;
	
	TArray<TTuple<FInventoryItems*, int32, int32> >InventoryItems;
	
	//================== Protected ==================//
protected:
#pragma region UE Methods
	// Sets default values for this actor's properties
	AStorageController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Item Data
	//		Item		   ,Stack ,Ammo  ,Inventory Ammo
	TTuple<FInventoryItems*, int32, int32> InventoryItem;

	int32 AmmoQuantity;
	
#pragma endregion

	void UpdateInteractionWidget();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
};

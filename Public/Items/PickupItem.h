// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemsBase.h"
#include "GameFramework/Actor.h"
#include "PickupItem.generated.h"

struct FInventoryItems;
class UBoxComponent;
class UInventoryComponent;
class USphereComponent;

UCLASS()
class LAGINVENTORY_API APickupItem : public AItemsBase
{
	GENERATED_BODY()

protected:
#pragma region UE Methods
	// Sets default values for this actor's properties
	APickupItem();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
#pragma endregion

#pragma region Item Data
	//		Item		   ,Stack ,Ammo  ,Inventory Ammo
	TTuple<FInventoryItems*, int32, int32> InventoryItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	FDataTableRowHandle InventoryData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	int32 ItemQuantity;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	int32 AmmoQuantity;
	UFUNCTION(CallInEditor, Category= "Data")
	void SetupItem();
	
#pragma endregion

	void UpdateInteractionWidget();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;
public:
#pragma region Drop
	void InitializeDrop(TTuple<FInventoryItems*, int32, int32> NewInventoryItem);
#pragma endregion

	virtual void Interact() override;

	bool IsItemValid() const;
};

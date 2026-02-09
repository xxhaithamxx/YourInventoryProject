// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InventoryCharacter.generated.h"

class UEquipmentComponent;
struct FInventoryItems;
class UCharacterStatsComponent;
class UInventoryComponent;

UCLASS()
class LAGINVENTORY_API AInventoryCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AInventoryCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	FORCEINLINE TObjectPtr<UInventoryComponent> GetInventory() const {return  InventoryComponent;}
	
	FORCEINLINE TObjectPtr<UCharacterStatsComponent> GetStatsComponent() const {return  CharacterStatsComponent;}

	void Equip(FInventoryItems* SlotDataHandler);
	void UnEquip(FInventoryItems* SlotDataHandler);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditAnywhere)
	UCharacterStatsComponent* CharacterStatsComponent;

	UPROPERTY(EditAnywhere)
	UEquipmentComponent* EquipmentComponent;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UserWidgets/Inventory/Slots/CharacterSlot.h"
#include "EquipAble.generated.h"

class UCharacterSlot;
class UEquipmentComponent;
struct FInventoryItems;
enum class EAmmoType : uint8;
enum class EWeaponSocket : uint8;
enum class EWeaponType : uint8;
enum class EEquipType : uint8;

UCLASS()
class LAGINVENTORY_API AEquipAble : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEquipAble();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	USceneComponent* Default;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UCharacterSlot* CharacterSlot;
	
	UPROPERTY()
	UEquipmentComponent* EquipmentComponent;
	
	virtual void AddItem(UCharacterSlot* CurrentCharacterSlot, UEquipmentComponent* CurrentEquipmentComponent);

	FORCEINLINE virtual TTuple<FInventoryItems*, int32, int32> GetInventoryItems()
	{
		return CharacterSlot->GetInventoryItems();
	}

	virtual UAnimMontage* GetAnimMontage();
	virtual float Attack();
	virtual FTransform GetSocket();
};

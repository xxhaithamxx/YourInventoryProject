// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipAble.h"
#include "GameFramework/Actor.h"
#include "MeleeWeapon.generated.h"

struct  FMeleeWeaponItems;

UCLASS()
class LAGINVENTORY_API AMeleeWeapon : public AEquipAble
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeleeWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
//FRangedWeaponItem* CurrentWeapon;
	FMeleeWeaponItems* CurrentWeapon;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AddItem(UCharacterSlot* CurrentCharacterSlot, UEquipmentComponent* CurrentEquipmentComponent) override;
	virtual float Attack() override;

	virtual UAnimMontage* GetAnimMontage() override;
};

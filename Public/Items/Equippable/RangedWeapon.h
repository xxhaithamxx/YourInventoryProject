// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipAble.h"
#include "GameFramework/Actor.h"
#include "RangedWeapon.generated.h"

struct FRangedWeaponItem;

UCLASS()
class LAGINVENTORY_API ARangedWeapon : public AEquipAble
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARangedWeapon();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FRangedWeaponItem* CurrentWeapon;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void AddItem(UCharacterSlot* CurrentCharacterSlot, UEquipmentComponent* CurrentEquipmentComponent) override;
	virtual float Attack() override;

	UPROPERTY()
	const USkeletalMeshSocket* Socket;
	
	virtual UAnimMontage* GetAnimMontage() override;
	virtual FTransform GetSocket() override;
};

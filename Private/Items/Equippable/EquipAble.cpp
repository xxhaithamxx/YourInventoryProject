// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/Equippable/EquipAble.h"

#include "UserWidgets/Inventory/Slots/CharacterSlot.h"


// Sets default values
AEquipAble::AEquipAble()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Default = CreateDefaultSubobject<USceneComponent>("Scene To Make Weapon Chiled");

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEquipAble::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEquipAble::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEquipAble::AddItem(UCharacterSlot* CurrentCharacterSlot, UEquipmentComponent* CurrentEquipmentComponent)
{
	CharacterSlot = CurrentCharacterSlot;
	EquipmentComponent = CurrentEquipmentComponent;
}

UAnimMontage* AEquipAble::GetAnimMontage()
{
	return nullptr;
}

float AEquipAble::Attack()
{
	return 0;
}

FTransform AEquipAble::GetSocket()
{
	return FTransform(FQuat::Identity, FVector::ZeroVector, FVector::OneVector);
}

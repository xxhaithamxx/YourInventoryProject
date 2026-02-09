// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/TestDropItem.h"

#include "Items/LootItem.h"


// Sets default values
ATestDropItem::ATestDropItem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestDropItem::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.bNoFail = true;
	SpawnParameters.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	const FVector SpawnLocation{GetActorLocation() + GetActorForwardVector() * 150.f};
	const FTransform SpawnTransform{GetActorRotation(), SpawnLocation};
	const ALootItem* Pickup = GetWorld()->SpawnActor<ALootItem>(LootItemClass, SpawnTransform, SpawnParameters);
	if (Pickup)
	{
		Pickup->SetLootItems(DropItems);
	}
	Pickup = nullptr;
}

// Called every frame
void ATestDropItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

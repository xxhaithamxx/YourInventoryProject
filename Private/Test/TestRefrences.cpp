// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestRefrences.h"
#include "Test/TestRefrenceHandler.h"


// Sets default values
ATestRefrences::ATestRefrences()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestRefrences::BeginPlay()
{
	Super::BeginPlay();

	InventoryItem = MakeTuple(InventoryData.GetRow<FInventoryItems>(TEXT("ContextString")), 0,0);
	Test1->AddeItem(&InventoryItem);
	Test2->AddeItem(&InventoryItem);
}

// Called every frame
void ATestRefrences::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


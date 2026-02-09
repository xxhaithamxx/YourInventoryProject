// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestConstractor.h"


// Sets default values
ATestConstractor::ATestConstractor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestConstractor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestConstractor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestConstractor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UE_LOG(LogTemp, Warning, TEXT("ATestConstractor::OnConstruction"));
}

void ATestConstractor::RerunConstructionScripts()
{
	Super::RerunConstructionScripts();
	UE_LOG(LogTemp, Warning, TEXT("ATestConstractor::RerunConstructionScripts"));
}


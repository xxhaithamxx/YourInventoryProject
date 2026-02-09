// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestDefaultRoot.h"


// Sets default values
ATestDefaultRoot::ATestDefaultRoot()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = Default = CreateDefaultSubobject<USceneComponent>("Scene");

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	MeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATestDefaultRoot::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATestDefaultRoot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Test/TestRefrenceHandler.h"

#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"


// Sets default values
ATestRefrenceHandler::ATestRefrenceHandler()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");

#pragma region BoxCollision
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(RootComponent);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
#pragma endregion

#pragma region SphereCollision
	SphereCollision = CreateDefaultSubobject<USphereComponent>("Collision");
	SphereCollision->SetupAttachment(RootComponent);
#pragma endregion
}

// Called when the game starts or when spawned
void ATestRefrenceHandler::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ATestRefrenceHandler::BeginOverlap);
}

void ATestRefrenceHandler::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	InventoryItem->Get<1>() += 1;
	UE_LOG(LogTemp, Warning, TEXT("ATestRefrenceHandler::BeginOverlap %d"),InventoryItem->Get<1>());
}

// Called every frame
void ATestRefrenceHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestRefrenceHandler::AddeItem(TTuple<FInventoryItems*, int32, int32>* InventoryItems)
{
	InventoryItem = InventoryItems;
	MeshComponent->SetSkeletalMesh(InventoryItem->Get<0>()->GetItemMesh());
}


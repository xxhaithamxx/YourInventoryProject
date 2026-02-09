// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemsBase.h"

#include "Component/InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"

// Sets default values
AItemsBase::AItemsBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

#pragma region UWidgetComponent ItemWidget
	ItemWidget = CreateDefaultSubobject<UWidgetComponent>("ItemWidget");
	ItemWidget->SetupAttachment(RootComponent);
	ItemWidget->SetVisibility(false);
#pragma endregion
}

// Called when the game starts or when spawned
void AItemsBase::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AItemsBase::BeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AItemsBase::EndOverlap);
}

void AItemsBase::BeginInteract()
{
}

void AItemsBase::EndInteract()
{
}

// Called every frame
void AItemsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Focus
void AItemsBase::BeginFocus()
{
	if (MeshComponent)
	{
		MeshComponent->SetRenderCustomDepth(true);
		if (ItemWidget)
		{
			ItemWidget->SetVisibility(true);
		}
	}
}

void AItemsBase::EndFocus()
{
	if (MeshComponent)
	{
		MeshComponent->SetRenderCustomDepth(false);
		if (ItemWidget)
		{
			ItemWidget->SetVisibility(false);
		}
	}
}
#pragma endregion

#pragma region Overlap
void AItemsBase::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Contains("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemsBase::BeginOverlap 1"));
		if (!InventoryPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("AItemsBase::BeginOverlap Set Player"));
			InventoryPlayerController = Cast<AInventoryPlayerController>(
				UGameplayStatics::GetPlayerController(GetWorld(), 0));
		}
		UE_LOG(LogTemp, Warning, TEXT("AItemsBase::BeginOverlap Add Overlap To player"));
		InventoryPlayerController->GetInventoryComponent()->AddOverlapItem(this);
	}
}

void AItemsBase::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Tags.Contains("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("AItemsBase::EndOverlap Remove Overlap from player"));
		InventoryPlayerController->GetInventoryComponent()->RemoveOverlapItem(this);
	}
}

void AItemsBase::Interact()
{
}
#pragma endregion

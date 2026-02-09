// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/VendorController.h"

#include "Blueprint/UserWidget.h"
#include "Component/InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"
#include "UserWidgets/Inventory/VendorPanel.h"

//================== Protected ==================// 
// Sets default values
AVendorController::AVendorController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("Vendor");
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
void AVendorController::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AVendorController::BeginOverlap);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AVendorController::EndOverlap);
}

// Called every frame
void AVendorController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVendorController::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Tags.Contains("Player"))
	{
		if (!InventoryPlayerController)
		{
			UE_LOG(LogTemp, Warning, TEXT("AVendorController::BeginOverlap"));
			InventoryPlayerController = Cast<AInventoryPlayerController>(
				UGameplayStatics::GetPlayerController(GetWorld(), 0));
		}
		InventoryPlayerController->GetInventoryComponent()->AddOverlapItem(this);
	}
}

void AVendorController::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Tags.Contains("Player"))
	{
		InventoryPlayerController->GetInventoryComponent()->RemoveOverlapItem(this);
	}
}

void AVendorController::Interact()
{
	UE_LOG(LogTemp, Warning, TEXT("AVendorController::Interact"));
	InventoryPlayerController->GetVendorPanel()->SetItems(this);
	InventoryPlayerController->ChangeLeftWidgetSwitcher(2);
	InventoryPlayerController->ToggleInGameWidget();
}

//================== Public ==================//

void AVendorController::BeginFocus()
{
	if (GetMesh())
	{
		GetMesh()->SetRenderCustomDepth(true);
		if (ItemWidget)
		{
			ItemWidget->SetVisibility(true);
		}
	}
	
	if (InventoryPlayerController)
	{
		InventoryPlayerController->OnInteract.AddDynamic(this, &AVendorController::Interact);
	}
}

void AVendorController::EndFocus()
{
	if (GetMesh())
	{
		GetMesh()->SetRenderCustomDepth(false);
		if (ItemWidget)
		{
			ItemWidget->SetVisibility(false);
		}
	}
	
	if (InventoryPlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("AVendorController::EndFocus"));
		InventoryPlayerController->OnInteract.RemoveDynamic(this, &AVendorController::Interact);
	}
}

void AVendorController::BeginInteract()
{
}

void AVendorController::EndInteract()
{
}

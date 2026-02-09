// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/InventoryComponent.h"

#include "Controller/InventoryCharacter.h"
#include "Interfaces/InteractableInterface.h"
#include "Items/ItemsBase.h"
#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	InventoryPlayerController->SetInventoryComponent(this);
	InventoryPlayerController->CreateInGame();
	//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::BeginPlay"));
	// ...
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddOverlapItem(TObjectPtr<AActor> Overlap)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::AddOverlapItem"));
	if (!OverlapItems.Contains(Overlap))
	{
		OverlapItems.Add(Overlap);
		//UE_LOG(LogTemp, Warning, TEXT("UInventoryComponent::AddOverlapItem2 %d"),OverlapItems.Num());
	}

	if (!GetWorld()->GetTimerManager().IsTimerActive(InteractTimerDelay))
	{
		GetWorld()->GetTimerManager().SetTimer(InteractTimerDelay, this, &UInventoryComponent::Interact, 0.5f, true);
	}
}

void UInventoryComponent::RemoveOverlapItem(TObjectPtr<AActor> Overlap)
{
	if (OverlapItems.Contains(Overlap))
	{
		OverlapItems.Remove(Overlap);
	}
	if (OverlapItems.Num() <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(InteractTimerDelay);
	}
}

void UInventoryComponent::Interact()
{
	FVector TraceStart{InventoryPlayerController->GetPlayerCharacter()->GetPawnViewLocation()};
	FVector TraceEnd{
		TraceStart + (InventoryPlayerController->GetPlayerCharacter()->GetViewRotation().Vector() * 250)
	};

	float LookDirection = FVector::DotProduct(InventoryPlayerController->GetPlayerCharacter()->GetActorForwardVector(),
	                                          InventoryPlayerController->GetPlayerCharacter()->GetViewRotation().
	                                                                     Vector());

	if (LookDirection > 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 1.0f, 0, 2.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(InventoryPlayerController->GetPlayerCharacter());

		if (FHitResult TraceHit; GetWorld()->LineTraceSingleByChannel(TraceHit, TraceStart, TraceEnd, ECC_Visibility,
		                                                              QueryParams))
		{
			if (IInteractableInterface* OverlapItem = Cast<IInteractableInterface>(TraceHit.GetActor()))
			{
				OverlapItem->BeginInteract();
				if (InteractionData.CurrentInteractable && OverlapItem == InteractionData.CurrentInteractable)
				{
					return;
				}

				if (InteractionData.CurrentInteractable && OverlapItem != InteractionData.CurrentInteractable)
				{
					InteractionData.CurrentInteractable->EndFocus();
				}
				InteractionData.CurrentInteractable = OverlapItem;
				InteractionData.CurrentInteractable->BeginFocus();
				return;
			}
		}
	}

	if (InteractionData.CurrentInteractable != nullptr)
	{
		InteractionData.CurrentInteractable->EndFocus();
		InteractionData.CurrentInteractable = nullptr;
	}
}

void UInventoryComponent::HealthTimer(EConsumableType& ConsumableType, float MaxDelay)
{
	float ElapsedTime = UGameplayStatics::GetRealTimeSeconds(GetWorld()); // Initialize the elapsed time to 0
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this,&ConsumableType ,ElapsedTime,MaxDelay]()
	{
		// Here you can specify the parameters you want to send with the delegate
		if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) >= MaxDelay + ElapsedTime)
		{
			OnHealthUsed.Broadcast(ConsumableType, ElapsedTime, MaxDelay);
			GetWorld()->GetTimerManager().ClearTimer(HealthTimerDelay);
		}
		// Broadcast the delegate with the specified parameters
		OnHealthUsed.Broadcast(ConsumableType, UGameplayStatics::GetRealTimeSeconds(this) - ElapsedTime, MaxDelay);
	});

	GetWorld()->GetTimerManager().SetTimer(HealthTimerDelay, TimerDelegate, 0.1f, true);
}

void UInventoryComponent::ManaTimer(EConsumableType& ConsumableType, float MaxDelay)
{
	float ElapsedTime = 0.0f; // Initialize the elapsed time to 0
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this,&ConsumableType ,&ElapsedTime,&MaxDelay]()
	{
		// Here you can specify the parameters you want to send with the delegate
		ElapsedTime += 0.1f;
		if (MaxDelay == ElapsedTime)
		{
			OnManaUsed.Broadcast(ConsumableType, ElapsedTime, MaxDelay);
			GetWorld()->GetTimerManager().ClearTimer(ManaTimerDelay);
		}
		// Broadcast the delegate with the specified parameters
		OnManaUsed.Broadcast(ConsumableType, ElapsedTime, MaxDelay);
	});

	GetWorld()->GetTimerManager().SetTimer(ManaTimerDelay, TimerDelegate, 0.1f, true);
}

void UInventoryComponent::StaminaTimer(EConsumableType& ConsumableType, float MaxDelay)
{
	float ElapsedTime = 0.0f; // Initialize the elapsed time to 0
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this,&ConsumableType ,&ElapsedTime,&MaxDelay]()
	{
		// Here you can specify the parameters you want to send with the delegate
		ElapsedTime += 0.1f;
		if (MaxDelay == ElapsedTime)
		{
			OnStaminaUsed.Broadcast(ConsumableType, ElapsedTime, MaxDelay);
			GetWorld()->GetTimerManager().ClearTimer(StaminaTimerDelay);
		}
		// Broadcast the delegate with the specified parameters
		OnStaminaUsed.Broadcast(ConsumableType, ElapsedTime, MaxDelay);
	});

	GetWorld()->GetTimerManager().SetTimer(StaminaTimerDelay, TimerDelegate, 0.1f, true);
}

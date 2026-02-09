// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CharacterStatsComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Settings/InventoryPlayerController.h"


// Sets default values for this component's properties
UCharacterStatsComponent::UCharacterStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InventoryPlayerController = Cast<AInventoryPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	InventoryPlayerController->SetCharacterStatsComponent(this);
}


// Called every frame
void UCharacterStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	
}

bool UCharacterStatsComponent::CanUseHeal()
{
	return CurrentHealth < MaxHealth;
}

void UCharacterStatsComponent::AddHeal(int32 Value)
{
	CurrentHealth += Value;
}

bool UCharacterStatsComponent::CanUseMana()
{
	return CurrentMana < MaxMana;
}

void UCharacterStatsComponent::AddMana(int32 Value)
{
	CurrentMana += Value;
}

bool UCharacterStatsComponent::CanUseStamina()
{
	return CurrentStamina < MaxStamina;
}

void UCharacterStatsComponent::AddStamina(int32 Value)
{
	CurrentStamina += Value;
}

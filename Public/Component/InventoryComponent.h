// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Structs/InteractionData.h"
#include "InventoryComponent.generated.h"

class IInteractableInterface;
class AInventoryPlayerController;
class AItemsBase;
enum class EConsumableType : uint8;
class UInventoryPanel;
enum class EAmmoType : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAddQuest, FInventoryItems&, ItemsData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoEvent, const EAmmoType&, AmmoType, int32, Count);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnConsumableUsed, const EConsumableType&, ConsumableType, float,
                                               CurrentDelay, float, MaxDelay);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDelayItem, FInventoryItems&, ItemsData, float,
                                               CurrentDelay, float, MaxDelay);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LAGINVENTORY_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInventoryPanel> InventoryPanels;

	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	TArray<TObjectPtr<AActor>> OverlapItems;

	UPROPERTY(EditAnywhere)
	int32 SlotsCapacity = 50;

	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FInteractionData InteractionData;
	
public:
	// Sets default values for this component's properties
	UInventoryComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { SlotsCapacity = NewSlotsCapacity; };
	FORCEINLINE int32 GetSlotsCapacity() const { return SlotsCapacity; }

	FORCEINLINE void AddOverlapItem(TObjectPtr<AActor> Overlap);
	FORCEINLINE void RemoveOverlapItem(TObjectPtr<AActor> Overlap);

	UFUNCTION()
	void Interact();

#pragma region  Timer Handle
	FTimerHandle HealthTimerDelay;
	FTimerHandle ManaTimerDelay;
	FTimerHandle StaminaTimerDelay;
	FTimerHandle InteractTimerDelay;
#pragma endregion

#pragma region Timer Handler
	UFUNCTION()
	void HealthTimer(EConsumableType& ConsumableType, float MaxDelay);
	UFUNCTION()
	void ManaTimer(EConsumableType& ConsumableType, float MaxDelay);
	UFUNCTION()
	void StaminaTimer(EConsumableType& ConsumableType, float MaxDelay);
#pragma endregion

	FOnDelayItem OnDelayItem;
	FOnAddQuest OnAddQuest;

#pragma region Ammo
	FOnAmmoEvent OnAmmoEvent;
#pragma endregion

#pragma region Consumable
	FOnConsumableUsed OnHealthUsed;
	FOnConsumableUsed OnManaUsed;
	FOnConsumableUsed OnStaminaUsed;
#pragma endregion
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatsComponent.generated.h"


class AInventoryPlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LAGINVENTORY_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCharacterStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float CurrentHealth = 50;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100;
	
	UPROPERTY(EditAnywhere)
	float CurrentMana = 50;

	UPROPERTY(EditAnywhere)
	float MaxMana = 100;

	UPROPERTY(EditAnywhere)
	float CurrentStamina = 50;

	UPROPERTY(EditAnywhere)
	float MaxStamina = 100;

	UPROPERTY(EditAnywhere)
	int32 Level = 1;
	
	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

	
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	bool CanUseHeal();
	void AddHeal(int32 Value);
	bool CanUseMana();
	void AddMana(int32 Value);
	bool CanUseStamina();
	void AddStamina(int32 Value);

	FORCEINLINE int32 PlayerLevel() const {return  Level;}
};

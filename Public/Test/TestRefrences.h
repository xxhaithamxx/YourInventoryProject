// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestRefrences.generated.h"

class ATestRefrenceHandler;
struct FInventoryItems;

UCLASS()
class LAGINVENTORY_API ATestRefrences : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestRefrences();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Item Data
	//		Item		   ,Stack ,Ammo  ,Inventory Ammo
	TTuple<FInventoryItems*, int32, int32> InventoryItem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	FDataTableRowHandle InventoryData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	ATestRefrenceHandler* Test1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	ATestRefrenceHandler* Test2;
#pragma endregion
};

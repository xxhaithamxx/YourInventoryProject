// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LAGInventory/DataBase/Structs/Items/InventoryItems.h"

#include "TestRefrenceHandler.generated.h"


class UBoxComponent;
class USphereComponent;

UCLASS()
class LAGINVENTORY_API ATestRefrenceHandler : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestRefrenceHandler();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Overlap

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#pragma endregion

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

	TTuple<FInventoryItems*, int32, int32>* InventoryItem;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddeItem(TTuple<FInventoryItems*, int32, int32>* InventoryItems);
};

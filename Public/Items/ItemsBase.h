// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "ItemsBase.generated.h"

class UWidgetComponent;
class USphereComponent;
class UBoxComponent;
class AInventoryPlayerController;

UCLASS()
class LAGINVENTORY_API AItemsBase : public AActor ,public IInteractableInterface
{
	GENERATED_BODY()

	//================== Public ==================//
public:
	
#pragma region Focus
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
#pragma endregion

#pragma region Overlap

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                          UPrimitiveComponent* OtherComp,
	                          int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion

	virtual void BeginInteract() override;
	virtual void EndInteract() override;

	UFUNCTION()
	virtual void Interact() override;

	//================== Protected ==================// 
protected:
	// Sets default values for this actor's properties
	AItemsBase();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> ItemWidget;
	
	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;
};

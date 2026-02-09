// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractableInterface.h"
#include "Structs/VendorSellItems.h"
#include "VendorController.generated.h"

class UBoxComponent;
class USphereComponent;
class UWidgetComponent;
class AInventoryPlayerController;
class UVendorPanel;

UCLASS()
class LAGINVENTORY_API AVendorController : public ACharacter,public IInteractableInterface
{
	GENERATED_BODY()

	//================== Protected ==================// 
protected:
	// Sets default values for this character's properties
	AVendorController();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;
	
#pragma region Overlap

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion

	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> ItemWidget;

	//================== Public ==================//
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Data | Database")
	TArray<FVendorSellItems> VendorSellItems;

	UFUNCTION()
	virtual void Interact() override;
	
#pragma region Focus
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
#pragma endregion
	
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
};

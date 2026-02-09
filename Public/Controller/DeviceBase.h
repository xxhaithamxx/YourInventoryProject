// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableInterface.h"
#include "DeviceBase.generated.h"

UCLASS()
class LAGINVENTORY_API ADeviceBase : public AActor ,public IInteractableInterface
{
	GENERATED_BODY()

	//================== Protected ==================// 
protected:
	// Sets default values for this actor's properties
	ADeviceBase();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FDataTableRowHandle LeaveMe;
	
#pragma region Overlap

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							  UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
#pragma endregion

	//================== Public ==================//
public:
#pragma region Focus
	virtual void BeginFocus() override;
	virtual void EndFocus() override;
#pragma endregion

#pragma region Interact
	virtual void BeginInteract() override;
	virtual void EndInteract() override;
	virtual void Interact() override;
#pragma endregion 
};

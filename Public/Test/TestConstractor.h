// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestConstractor.generated.h"

UCLASS()
class LAGINVENTORY_API ATestConstractor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestConstractor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void RerunConstructionScripts() override;

};

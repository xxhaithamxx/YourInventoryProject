// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestDefaultRoot.generated.h"

UCLASS()
class LAGINVENTORY_API ATestDefaultRoot : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestDefaultRoot();

	UPROPERTY(EditAnywhere)
	USceneComponent* Default;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USkeletalMeshComponent> MeshComponent;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

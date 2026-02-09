// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemsBase.h"
#include "GameFramework/Actor.h"
#include "Structs/RandomLootDrop.h"
#include "LootItem.generated.h"


UCLASS()
class LAGINVENTORY_API ALootItem : public AItemsBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALootItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class ULootPanel* LootPanel;

	UPROPERTY(EditAnywhere, Category= "Widgets")
	TSubclassOf<ULootPanel> LootPanelClass;

	virtual void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	bool IsDestroy=false;
public:

	virtual void Interact() override;

	void SetLootItems(TArray<FRandomLootDrop> DropItems) const;
	
	void LootedAllItem();

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

};

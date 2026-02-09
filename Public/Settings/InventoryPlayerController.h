// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALSPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "InventoryPlayerController.generated.h"

class UStoragePanel;
class UInGameWidget;
class UVendorPanel;
class UCharacterPanel;
class AGameHUD;
class UCharacterStatsComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

class UInputMappingContext;
struct FInputActionValue;
class UInventoryPanel;
class UInventoryGameWidget;
class UEquipmentComponent;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API AInventoryPlayerController : public AALSPlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TObjectPtr<ACharacter> PlayerCharacter;

	UPROPERTY()
	TArray<UUserWidget*> UIActive;

	void ShowInGameWidget();
	void HideInGameWidget();

#pragma region component
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY()
	TObjectPtr<UEquipmentComponent> EquipmentComponent;

	UPROPERTY()
	TObjectPtr<UCharacterStatsComponent> CharacterStatsComponent;
#pragma endregion

#pragma region Widgets
	UPROPERTY()
	TObjectPtr<UInventoryGameWidget> InventoryGameWidget;

	UPROPERTY()
	TObjectPtr<UInGameWidget> InGameWidget;

	UPROPERTY()
	TObjectPtr<UInventoryPanel> InventoryPanel;

	UPROPERTY()
	TObjectPtr<UStoragePanel> StoragePanel;

	UPROPERTY()
	TObjectPtr<UCharacterPanel> CharacterPanel;

	UPROPERTY()
	TObjectPtr<UVendorPanel> VendorPanel;
#pragma endregion

#pragma region Inventory Inputs
	UFUNCTION()
	void InteractAction(const FInputActionValue& Value);

	UFUNCTION()
	void InventoryAction(const FInputActionValue& Value);
#pragma endregion

#pragma region Inventory Inputs
	UFUNCTION()
	void MeleeSwitchAction(const FInputActionValue& Value);
	UFUNCTION()
	void PrimarySwitchAction(const FInputActionValue& Value);
	UFUNCTION()
	void SecondarySwitchAction(const FInputActionValue& Value);
#pragma endregion
	
	UPROPERTY()
	TObjectPtr<AInventoryPlayerController> InventoryPlayerController;

	virtual void BeginPlay() override;

public:
	virtual void OnPossess(APawn* InPawn) override;
	void CreateInGame();

#pragma region Inputs
	virtual void SetupInputComponent() override;
	void BindActions(TObjectPtr<UInputMappingContext> Context);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Input")
	TObjectPtr<UInputMappingContext> InventoryInputMappingContext = nullptr;
#pragma endregion

	FOnInteract OnInteract;

	TObjectPtr<ACharacter> GetPlayerCharacter() { return PlayerCharacter; }

#pragma region Getter And Setter Component
	TObjectPtr<UInGameWidget> GetInGameWidget() const { return InGameWidget; }

	void SetInGameWidget(const TObjectPtr<UInGameWidget> CurrentInGameWidget) { InGameWidget = CurrentInGameWidget; }

	TObjectPtr<UInventoryComponent> GetInventoryComponent() const { return InventoryComponent; }

	void SetInventoryComponent(const TObjectPtr<UInventoryComponent> Inventory) { InventoryComponent = Inventory; }

	TObjectPtr<UEquipmentComponent> GetEquipmentComponent() const { return EquipmentComponent; }

	void SetEquipmentComponent(const TObjectPtr<UEquipmentComponent> Equipment) { EquipmentComponent = Equipment; }

	TObjectPtr<UCharacterStatsComponent> GetCharacterStatsComponent() const { return CharacterStatsComponent; }

	void SetCharacterStatsComponent(const TObjectPtr<UCharacterStatsComponent> Equipment)
	{
		CharacterStatsComponent = Equipment;
	}
#pragma endregion

#pragma region Getter And Setter Widgets
	TObjectPtr<UInventoryGameWidget> GetInventoryGameWidget() const { return InventoryGameWidget; }

	void SetInventoryGameWidget(const TObjectPtr<UInventoryGameWidget> CurrentInventoryPanel)
	{
		UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::SetInventoryGameWidget"));
		InventoryGameWidget = CurrentInventoryPanel;
	}

	TObjectPtr<UInventoryPanel> GetInventoryPanel() const { return InventoryPanel; }

	void SetInventoryPanel(const TObjectPtr<UInventoryPanel> CurrentInventoryPanel)
	{
		InventoryPanel = CurrentInventoryPanel;
	}

	TObjectPtr<UStoragePanel> GetStoragePanel() const { return StoragePanel; }

	void SetStoragePanel(const TObjectPtr<UStoragePanel> CurrentStoragePanel) { StoragePanel = CurrentStoragePanel; }

	TObjectPtr<UCharacterPanel> GetCharacterPanel() const { return CharacterPanel; }

	void SetCharacterPanel(const TObjectPtr<UCharacterPanel> Current) { CharacterPanel = Current; }

	TObjectPtr<UVendorPanel> GetVendorPanel() const { return VendorPanel; }

	void SetVendorPanel(const TObjectPtr<UVendorPanel> Current) { VendorPanel = Current; }
	//
#pragma endregion

	void ToggleInGameWidget();

	UPROPERTY(EditAnywhere, Category= "Widgets")
	TSubclassOf<UInventoryGameWidget> InventoryGameClass;

	UPROPERTY(EditAnywhere, Category= "Widgets")
	TSubclassOf<UInGameWidget> InGameClass;

	FORCEINLINE void ClearUIActive() { UIActive.Empty(); }
	void RemoveUIActive(UUserWidget* UI);
	void AddUIActive(UUserWidget* UI);

	void ChangeLeftWidgetSwitcher(int32 NewID);
};

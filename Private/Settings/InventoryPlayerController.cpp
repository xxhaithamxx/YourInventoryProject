// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/InventoryPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Component/EquipmentComponent.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/Character.h"
#include "UserWidgets/InventoryGameWidget.h"
#include "UserWidgets/InGameWidget.h"

#pragma region Inventory Inputs
void AInventoryPlayerController::InteractAction(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		OnInteract.Broadcast();
	}
}

void AInventoryPlayerController::InventoryAction(const FInputActionValue& Value)
{
	if (Value.Get<bool>())
	{
		UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::InventoryAction"));
		InventoryGameWidget->ChangeLeftWidgetSwitcher(0);
		ToggleInGameWidget();	
	}
}

void AInventoryPlayerController::MeleeSwitchAction(const FInputActionValue& Value)
{
	if (EquipmentComponent)
	{
		EquipmentComponent->SwitchWeapon(1);
	}
}

void AInventoryPlayerController::PrimarySwitchAction(const FInputActionValue& Value)
{
	if (EquipmentComponent)
	{
		EquipmentComponent->SwitchWeapon(2);
	}
}

void AInventoryPlayerController::SecondarySwitchAction(const FInputActionValue& Value)
{
	if (EquipmentComponent)
	{
		EquipmentComponent->SwitchWeapon(3);
	}
}
#pragma endregion

void AInventoryPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	PlayerCharacter = Cast<ACharacter>(InPawn);
	UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::OnPossess"));
	PlayerCharacter->Tags.Add("Player");
}

void AInventoryPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->ClearActionEventBindings();
		EnhancedInputComponent->ClearActionValueBindings();
		EnhancedInputComponent->ClearDebugKeyBindings();
		UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::SetupInputComponent"));
		BindActions(InventoryInputMappingContext);
		BindActions(DefaultInputMappingContext);
		BindActions(DebugInputMappingContext);
	}
	else
	{
		UE_LOG(LogTemp, Fatal, TEXT("AInventoryPlayerController::SetupInputComponent requires Enhanced Input System to be activated in project settings to function properly"));
	}
}

void AInventoryPlayerController::BindActions(TObjectPtr<UInputMappingContext> Context)
{
	if (Context)
	{
		UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::BindActions"));
		const TArray<FEnhancedActionKeyMapping>& Mappings = Context->GetMappings();
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::BindActions if"));
			// There may be more than one keymapping assigned to one action. So, first filter duplicate action entries to prevent multiple delegate bindings
			TSet<const UInputAction*> UniqueActions;
			for (const FEnhancedActionKeyMapping& Keymapping : Mappings)
			{
				UniqueActions.Add(Keymapping.Action);	
			}
			for (const UInputAction* UniqueAction : UniqueActions)
			{
				EnhancedInputComponent->BindAction(UniqueAction, ETriggerEvent::Triggered, Cast<UObject>(this), UniqueAction->GetFName());
			}
		}
	}
}

void AInventoryPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		FModifyContextOptions Options;
		Options.bForceImmediately = 1;
		Subsystem->AddMappingContext(InventoryInputMappingContext, 1, Options);
	}
}

void AInventoryPlayerController::CreateInGame()
{
	if (InventoryGameClass)
	{
		InventoryGameWidget = CreateWidget<UInventoryGameWidget>(GetWorld(), InventoryGameClass);
		InventoryGameWidget->AddToViewport(5);
		InventoryGameWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InGameClass)
	{
		InGameWidget = CreateWidget<UInGameWidget>(GetWorld(), InGameClass);
		InGameWidget->AddToViewport(-5);
		InGameWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AInventoryPlayerController::ToggleInGameWidget()
{
	if (InventoryGameWidget->IsVisible())
	{
		HideInGameWidget();
	}
	else
	{
		ShowInGameWidget();
	}
}

void AInventoryPlayerController::ShowInGameWidget() 
{
	UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::ShowInGameWidget"));
	AddUIActive(InventoryGameWidget);
	InventoryGameWidget->SetVisibility(ESlateVisibility::Visible);
}

void AInventoryPlayerController::HideInGameWidget() 
{
	UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::HideInGameWidget"));
	RemoveUIActive(InventoryGameWidget);
	InventoryGameWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void AInventoryPlayerController::RemoveUIActive(UUserWidget* UI)
{
	if (UIActive.Contains(UI))
	{
		UIActive.Remove(UI);
		if (UIActive.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::RemoveUIActive"));
			SetShowMouseCursor(false);
			const FInputModeGameOnly InputMode;
			SetInputMode(InputMode);
		}
	}
}

void AInventoryPlayerController::AddUIActive(UUserWidget* UI)
{
	if (!UIActive.Contains(UI))
	{
		FInputModeUIOnly InputMode;
		if (UIActive.Num() == 0)
		{
			InputMode.SetWidgetToFocus(UI->TakeWidget());
		}
		UIActive.Add(UI);
		
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
		UE_LOG(LogTemp, Warning, TEXT("AInventoryPlayerController::AddUIActive After %d"), UIActive.Num());
	}
}

void AInventoryPlayerController::ChangeLeftWidgetSwitcher(int32 NewID)
{
	InventoryGameWidget->ChangeLeftWidgetSwitcher(NewID);
}

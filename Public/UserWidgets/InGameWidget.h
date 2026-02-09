// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

class UVerticalBox;
class UItemsNotifications;
/**
 * 
 */
UCLASS()
class LAGINVENTORY_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

	//================== Public ==================//
public:
	UPROPERTY(EditAnywhere,Category= "Widgets")
	TSubclassOf<UItemsNotifications> ItemsNotificationsClass ;

	void ShowNotifications(UTexture2D* Icon,FText Msg);

	UPROPERTY(meta=(BindWidget))
	UVerticalBox* NotificationsParent;
	//================== Protected ==================//
protected:
};

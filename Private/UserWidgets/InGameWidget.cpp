// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgets/InGameWidget.h"

#include "Components/VerticalBox.h"
#include "UserWidgets/Mics/ItemsNotifications.h"

void UInGameWidget::ShowNotifications(UTexture2D* Icon, FText Msg)
{
	if (ItemsNotificationsClass)
	{
		UItemsNotifications* NotificationMsgWidget = CreateWidget<UItemsNotifications>(GetWorld(), ItemsNotificationsClass);
		NotificationMsgWidget->SetNotifications(Icon,Msg);
		NotificationsParent->AddChild(NotificationMsgWidget);
		NotificationMsgWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

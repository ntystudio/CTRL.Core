// Fill out your copyright notice in the Description page of Project Settings.

#include "CTRLCore/CTRLWidget.h"

#include "Components/WidgetComponent.h"

#include "Templates/SubclassOf.h"

UUserWidget* UCTRLWidget::K2_GetWidgetFromWidgetComponent(UWidgetComponent* WidgetComponent, TSubclassOf<UUserWidget> const WidgetClass, bool& IsWidgetValid)
{
	IsWidgetValid = false;
	if (!WidgetComponent) return nullptr;
	if (auto const Widget = WidgetComponent->GetUserWidgetObject())
	{
		if (Widget->IsA(WidgetClass))
		{
			IsWidgetValid = true;
			return Widget;
		}
	}
	return nullptr;
}

template <typename T>
T* UCTRLWidget::GetWidgetFromWidgetComponent(UWidgetComponent* WidgetComponent)
{
	if (!WidgetComponent) return nullptr;
	return Cast<T>(WidgetComponent->GetUserWidgetObject());
}

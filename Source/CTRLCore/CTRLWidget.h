// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CTRLWidget.generated.h"

class UUserWidget;
class UWidgetComponent;

/**
 * UMG Widget Component Utility Functions
 */
UCLASS(Category="CTRL|Widget", DisplayName="Widget Utils [CTRL]")
class CTRLCORE_API UCTRLWidget : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Get Widget From WidgetComponent [CTRL]",
		Category = "CTRL|Widget",
		meta = (
			Keywords = "Component UMG UserWidget UUserWidget UI",
			ComponentClass = "/Script/UMG.WidgetComponent",
			DeterminesOutputType = "WidgetClass",
			ExpandBoolAsExecs = "IsWidgetValid",
			DefaultToSelf = "WidgetComponent"
		)
	)
	static UUserWidget* K2_GetWidgetFromWidgetComponent(
		UWidgetComponent* WidgetComponent,
		TSubclassOf<UUserWidget> const WidgetClass,
		bool& IsWidgetValid
	);

	template <typename T = UUserWidget>
	static T* GetWidgetFromWidgetComponent(UWidgetComponent* WidgetComponent);
};

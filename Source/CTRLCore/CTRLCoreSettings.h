// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CTRLCore/CTRLMsgLog.h"

#include "Engine/DeveloperSettings.h"

#include "CTRLCoreSettings.generated.h"

/**
 * 
 */
UCLASS(DefaultConfig, Config="Editor", meta=(DisplayName="CTRL.Core"))
class CTRLCORE_API UCTRLCoreEditorContainerSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Config, meta=(ShowOnlyInnerProperties))
	FCTRLMsgLogSettings MsgLogSettings;
	

#if WITH_EDITORONLY_DATA
	virtual FName GetContainerName() const override { return TEXT("Editor"); }
	virtual FName GetCategoryName() const override { return TEXT("CTRL"); }
	virtual FText GetSectionText() const override { return FText::FromString(TEXT("Core")); }
#endif
};

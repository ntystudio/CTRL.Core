// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Subsystems/EngineSubsystem.h"

#include "CTRLPrimaryAssetLoadingSubsystem.generated.h"

struct FStreamableHandle;
/**
 * This ensures any BP-subclasses of passed-in classes are loaded at editor/game startup and before entering PIE.
 * Make sure to configure an entry for the entries in "Project Settings" → "Asset Manager" → "Primary Asset Types to Scan"
 */
UCLASS(NotBlueprintType)
class CTRLCORE_API UCTRLPrimaryAssetLoadingSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void LoadAssets();

	// e.g AssetTypes.Add(UMyAsset::StaticClass()->GetFName())
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPrimaryAssetType> AssetTypes;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	TArray<TSharedPtr<FStreamableHandle>> LoadingHandles;

	void OnInitialScanComplete();
	void OnAssetAdded(FAssetData const& AssetData);
	void PostEngineInit();
	void PreBeginPIE(bool bIsSimulating);
};

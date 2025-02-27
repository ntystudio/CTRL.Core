// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserItemData.h"
#include "Editor.h"
#include "EditorSubsystem.h"

#include "Engine/DeveloperSettings.h"

#include "ThumbnailRendering/StaticMeshThumbnailRenderer.h"

#include "UObject/Interface.h"

#include "CTRLThumbnailRenderer.generated.h"

class UStaticMesh;

/**
 * 
 */
UCLASS()
class CTRLCOREEDITOR_API UCTRLStaticMeshThumbnailProviderRenderer : public UStaticMeshThumbnailRenderer
{
	GENERATED_BODY()

public:
	virtual bool CanVisualizeAsset(UObject* Object) override;

	virtual void Draw(UObject* Object, int32 const X, int32 const Y, uint32 const Width, uint32 const Height, FRenderTarget* Target, FCanvas* Canvas, bool const bAdditionalViewFamily) override;
};

/**
 * This ensures any BP-subclasses of UCTRLWorldSubsystemBlueprintBase are loaded at editor/game startup and before entering PIE.
 * Make sure to configure an entry for CTRLWorldSubsystemBlueprintBase in "Project Settings" → "Asset Manager" → "Primary Asset Types to Scan"
 */
UCLASS(NotBlueprintType)
class CTRLCOREEDITOR_API UCTRLThumbnailSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	static ThisClass* Get();
	void RegisterThumbnailRenderersInternal();
	void RegisterThumbnailRenderers();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UPROPERTY(Transient)
	TSet<TObjectPtr<UClass>> RegisteredAssetTypes;
	FTimerHandle QueuedUpdateHandle;

protected:
	void OnAssetsAdded(TArrayView<FAssetData const> AssetDatas);
	void OnInitialScanComplete();
	void OnItemDataUpdated(TArrayView<FContentBrowserItemDataUpdate const> ContentBrowserItemDataUpdates);
	void PostEngineInit();
};

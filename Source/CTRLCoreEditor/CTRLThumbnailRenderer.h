// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContentBrowserItemData.h"
#include "Editor.h"
#include "EditorSubsystem.h"

#include "ThumbnailRendering/StaticMeshThumbnailRenderer.h"

#include "CTRLThumbnailRenderer.generated.h"

class UStaticMesh;

/**
 * Takes an object that implements ICTRLStaticMeshThumbnailProvider, and uses GetThumbnailMesh() to render a static mesh thumbnail.
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
 * Handles registering thumbnail visualizers for ICTRLStaticMeshThumbnailProvider types.
 * @see https://zomgmoz.tv/unreal/Editor-customization/Custom-asset-editor-thumbnails
 */
UCLASS(NotBlueprintType)
class CTRLCOREEDITOR_API UCTRLThumbnailSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	static ThisClass* Get();
	void RegisterThumbnailRenderers();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	void RegisterThumbnailRenderersInternal();
	void OnAssetsAdded(TArrayView<FAssetData const> AssetDatas);
	void OnInitialScanComplete();
	void OnItemDataUpdated(TArrayView<FContentBrowserItemDataUpdate const> ContentBrowserItemDataUpdates);
	void PostEngineInit();
};

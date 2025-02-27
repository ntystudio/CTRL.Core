// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#include "CTRLThumbnailRenderer.h"

#include "ContentBrowserDataSubsystem.h"

#include "CTRLCore/CTRLAssetThumbnails.h"

#include "Engine/StaticMesh.h"

#include "ThumbnailRendering/ThumbnailManager.h"

#include "UObject/UObjectIterator.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CTRLThumbnailRenderer)

//~ ──────────────────────────── UCTRLThumbnailSubsystem ───────────────────────────── ~//

UCTRLThumbnailSubsystem::ThisClass* UCTRLThumbnailSubsystem::Get()
{
	if (!GEditor) return nullptr;
	return GEditor->GetEditorSubsystem<ThisClass>();
}

void UCTRLThumbnailSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UContentBrowserDataSubsystem>();
	Super::Initialize(Collection);
	FCoreDelegates::OnPostEngineInit.AddUObject(this, &ThisClass::PostEngineInit);
}

void UCTRLThumbnailSubsystem::RegisterThumbnailRenderers()
{
	// debounce registration to next editor tick
	if (!GEditor) return;
	if (QueuedUpdateHandle.IsValid()) return;
	QueuedUpdateHandle = GEditor->GetTimerManager()->SetTimerForNextTick(this, &ThisClass::RegisterThumbnailRenderersInternal);
}

void UCTRLThumbnailSubsystem::RegisterThumbnailRenderersInternal()
{
	QueuedUpdateHandle.Invalidate();
	if (!GIsEditor) return;

	// Find Classes Implementing UCTRLStaticMeshThumbnailProvider
	TSet<UClass*> Classes;
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		if (ClassIt->ImplementsInterface(UCTRLStaticMeshThumbnailProvider::StaticClass()))
		{
			Classes.Add(*ClassIt);
		}
	}

	// Register Thumbnail Renderers
	auto& ThumbnailManager = UThumbnailManager::Get();
	for (auto const AssetType : Classes)
	{
		if (!AssetType) continue;
		ThumbnailManager.UnregisterCustomRenderer(AssetType); // Don't bother tracking what we've already registered, just reregister everything
		ThumbnailManager.RegisterCustomRenderer(AssetType, UCTRLStaticMeshThumbnailProviderRenderer::StaticClass());
	}
}

void UCTRLThumbnailSubsystem::PostEngineInit()
{
	if (!GEditor) return;
	// If we register too early e.g. PostEngineInit, the thumbnails tend to go blank? The below hooks seem to be reliable though.
	if (auto const ContentBrowser = GEditor->GetEditorSubsystem<UContentBrowserDataSubsystem>())
	{
		ContentBrowser->OnItemDataDiscoveryComplete().AddUObject(this, &ThisClass::RegisterThumbnailRenderers);
		ContentBrowser->OnItemDataRefreshed().AddUObject(this, &ThisClass::RegisterThumbnailRenderers);
		ContentBrowser->OnItemDataUpdated().AddUObject(this, &ThisClass::OnItemDataUpdated);
	}
}

void UCTRLThumbnailSubsystem::Deinitialize()
{
	Super::Deinitialize();
	// we should probably unregister but whatever
}

void UCTRLThumbnailSubsystem::OnAssetsAdded(TArrayView<FAssetData const> AssetDatas)
{
	RegisterThumbnailRenderers();
}

void UCTRLThumbnailSubsystem::OnItemDataUpdated(TArrayView<FContentBrowserItemDataUpdate const> ContentBrowserItemDataUpdates)
{
	RegisterThumbnailRenderers();
}

//~ ────────────────────── StaticMeshThumbnailProviderRenderer ─────────────────────── ~//

bool UCTRLStaticMeshThumbnailProviderRenderer::CanVisualizeAsset(UObject* Object)
{
	if (auto const AsThumbnailProvider = Cast<ICTRLStaticMeshThumbnailProvider>(Object))
	{
		return IsValid(AsThumbnailProvider->GetThumbnailMesh());
	}
	return false;
}

void UCTRLStaticMeshThumbnailProviderRenderer::Draw(
	UObject* Object,
	int32 const X,
	int32 const Y,
	uint32 const Width,
	uint32 const Height,
	FRenderTarget* Target,
	FCanvas* Canvas,
	bool const bAdditionalViewFamily
)
{
	if (auto const* Stage = Cast<ICTRLStaticMeshThumbnailProvider>(Object))
	{
		if (auto const ThumbnailMesh = Stage->GetThumbnailMesh())
		{
			Super::Draw(ThumbnailMesh, X, Y, Width, Height, Target, Canvas, bAdditionalViewFamily);
		}
	}
}

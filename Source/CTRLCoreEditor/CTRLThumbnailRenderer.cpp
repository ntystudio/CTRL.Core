// Fill out your copyright notice in the Description page of Project Settings.

#include "CTRLCoreEditor/CTRLThumbnailRenderer.h"

#include "ContentBrowserDataSubsystem.h"

#include "CTRLCore/CTRLAssetThumbnails.h"

#include "Engine/StaticMesh.h"

#include "ThumbnailRendering/ThumbnailManager.h"

#include "UObject/UObjectIterator.h"

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

UCTRLThumbnailSubsystem::ThisClass* UCTRLThumbnailSubsystem::Get()
{
	if (GEditor)
	{
		return GEditor->GetEditorSubsystem<ThisClass>();
	}
	return nullptr;
}

void UCTRLThumbnailSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<UContentBrowserDataSubsystem>();
	Super::Initialize(Collection);
	FCoreDelegates::OnPostEngineInit.AddUObject(this, &ThisClass::PostEngineInit);
}

void UCTRLThumbnailSubsystem::PostEngineInit()
{
	if (!GEditor) return;
	if (auto const ContentBrowser = GEditor->GetEditorSubsystem<UContentBrowserDataSubsystem>())
	{
		ContentBrowser->OnItemDataDiscoveryComplete().AddUObject(this, &ThisClass::RegisterThumbnailRenderers);
		ContentBrowser->OnItemDataRefreshed().AddUObject(this, &ThisClass::RegisterThumbnailRenderers);
		ContentBrowser->OnItemDataUpdated().AddUObject(this, &ThisClass::OnItemDataUpdated);
	}
}

void UCTRLThumbnailSubsystem::RegisterThumbnailRenderers()
{
	if (!GEditor) return;
	if (QueuedUpdateHandle.IsValid()) return;
	QueuedUpdateHandle = GEditor->GetTimerManager()->SetTimerForNextTick(this, &ThisClass::RegisterThumbnailRenderersInternal);
}

void UCTRLThumbnailSubsystem::RegisterThumbnailRenderersInternal()
{
	QueuedUpdateHandle.Invalidate();
	if (!GIsEditor) return;
	TSet<UClass*> Classes;
	for (TObjectIterator<UClass> ClassIt; ClassIt; ++ClassIt)
	{
		if (!ClassIt->ImplementsInterface(UCTRLStaticMeshThumbnailProvider::StaticClass())) continue;

		Classes.Add(*ClassIt);
	}

	auto& ThumbnailManager = UThumbnailManager::Get();
	for (auto const AssetType : Classes)
	{
		if (!AssetType) continue;
		ThumbnailManager.UnregisterCustomRenderer(AssetType);
		ThumbnailManager.RegisterCustomRenderer(AssetType, UCTRLStaticMeshThumbnailProviderRenderer::StaticClass());
	}
}

void UCTRLThumbnailSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UCTRLThumbnailSubsystem::OnAssetsAdded(TArrayView<FAssetData const> AssetDatas)
{
	RegisterThumbnailRenderers();
}

void UCTRLThumbnailSubsystem::OnInitialScanComplete()
{
	RegisterThumbnailRenderers();
}

void UCTRLThumbnailSubsystem::OnItemDataUpdated(TArrayView<FContentBrowserItemDataUpdate const> ContentBrowserItemDataUpdates)
{
	RegisterThumbnailRenderers();
}

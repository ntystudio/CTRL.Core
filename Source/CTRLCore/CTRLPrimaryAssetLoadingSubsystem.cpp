// Fill out your copyright notice in the Description page of Project Settings.

#include "CTRLCore/CTRLPrimaryAssetLoadingSubsystem.h"

#include "ContentBrowserDataSubsystem.h"
#include "Editor.h"

#include "CTRLCore/CTRLWorldSubsystemBlueprintBase.h"

#include "Engine/AssetManager.h"

#include "Misc/CoreDelegates.h"

void UCTRLPrimaryAssetLoadingSubsystem::PreBeginPIE(bool bIsSimulating)
{
	LoadAssets();
}

void UCTRLPrimaryAssetLoadingSubsystem::OnInitialScanComplete()
{
	LoadAssets();
}

void UCTRLPrimaryAssetLoadingSubsystem::OnAssetAdded(FAssetData const& AssetData)
{
	LoadAssets();
}

void UCTRLPrimaryAssetLoadingSubsystem::PostEngineInit()
{
	UAssetManager& AssetManager = UAssetManager::Get();
	AssetManager.CallOrRegister_OnCompletedInitialScan(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnInitialScanComplete));
	if (IAssetRegistry* AssetRegistry = IAssetRegistry::Get())
	{
		AssetRegistry->OnFilesLoaded().AddUObject(this, &ThisClass::OnInitialScanComplete);
		AssetRegistry->OnAssetAdded().AddUObject(this, &ThisClass::OnAssetAdded);
	}
}

void UCTRLPrimaryAssetLoadingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// This should always happen before PostEngineInit
	FCoreDelegates::OnPostEngineInit.AddUObject(this, &ThisClass::PostEngineInit);

#if WITH_EDITOR
	if (GIsEditor)
	{
		// ensure subclasses get loaded before PIE
		FEditorDelegates::PreBeginPIE.AddUObject(this, &ThisClass::PreBeginPIE);
	}
#endif
}

void UCTRLPrimaryAssetLoadingSubsystem::Deinitialize()
{
	for (auto& LoadingHandle : LoadingHandles)
	{
		if (LoadingHandle.IsValid())
		{
			LoadingHandle->CancelHandle();
		}
	}
	LoadingHandles.Reset();
	Super::Deinitialize();
}

void UCTRLPrimaryAssetLoadingSubsystem::LoadAssets()
{
	auto const OldLoadingHandles = LoadingHandles;
	ON_SCOPE_EXIT
	{
		// clear old handle at end to prevent cancel then re-add
		for (auto& OldLoadingHandle : OldLoadingHandles)
		{
			if (OldLoadingHandle.IsValid())
			{
				OldLoadingHandle->CancelHandle();
			}
		}
	};

	LoadingHandles.Reset();
	for (auto const& AssetType : AssetTypes)
	{
		auto&& LoadingHandle = UAssetManager::Get().LoadPrimaryAssetsWithType(AssetType);
		if (!LoadingHandle.IsValid()) return; // e.g. all already loaded
		LoadingHandles.Add(LoadingHandle);
	}

	for (auto const& LoadingHandle : LoadingHandles)
	{
		LoadingHandle->WaitUntilComplete();
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

#include "CTRLCore.h"

// #include "CTRLAssets.h"

#include "Misc/CoreDelegates.h"

#define LOCTEXT_NAMESPACE "FCTRLCoreModule"

class FAssetRegistryModule;
DEFINE_LOG_CATEGORY(LogCTRLCore);

void FCTRLCoreModule::StartupModule()
{
	// GetMutableDefault<UCTRLAssetsSettings>()->Register();
}

void FCTRLCoreModule::ShutdownModule()
{
	// GetMutableDefault<UCTRLAssetsSettings>()->Unregister();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCTRLCoreModule, CTRLCore)

// #include "CTRLAssets.h"
//
// #include "AssetRegistry/AssetData.h"
//
// #include "CTRLCore/CTRLWorldSubsystemBlueprintBase.h"
//
// #include "Engine/AssetManager.h"
//
// #include "Misc/CoreDelegates.h"
//
// DEFINE_LOG_CATEGORY_CLASS(UCTRLAssetsSettings, LogCTRLAssets)
//
// UCTRLAssetsSettings::UCTRLAssetsSettings()
// {
// }
//
// void UCTRLAssetsSettings::OnPostEngineInit()
// {
// 	Load();
// }
//
// void UCTRLAssetsSettings::Load()
// {
// 	UE_LOG(LogCTRLAssets, Warning, TEXT("Load PrimaryAssetTypesToAlwaysLoad: %d. prev handles: %d"), PrimaryAssetTypesToAlwaysLoad.Num(), AllLoadingHandles.Num());
// 	TArray<TSharedPtr<FStreamableHandle>> LoadingHandles;
// 	LoadingHandles.Reserve(AllLoadingHandles.Num());
// 	for (auto const AssetType : PrimaryAssetTypesToAlwaysLoad)
// 	{
// 		TSharedPtr<FStreamableHandle> Handle = UAssetManager::Get().LoadPrimaryAssetsWithType(AssetType);
//
// 		if (Handle.IsValid())
// 		{
// 			LoadingHandles.Add(Handle);
// 		}
// 	}
// 	UE_LOG(LogCTRLAssets, Warning, TEXT("Load LoadingHandles %d"), LoadingHandles.Num());
// 	AllLoadingHandles = LoadingHandles; // only reassign after started loading all new (in case it causes existing load to abort on clear?)
// 	for (auto const Handle : LoadingHandles)
// 	{
// 		Handle->WaitUntilComplete();
// 	}
// }
//
// TArray<FPrimaryAssetType> UCTRLAssetsSettings::FindRelevantPrimaryAssetTypes(TArrayView<FAssetData const> const AssetDatas) const
// {
// 	TArray<FPrimaryAssetType> NewPrimaryAssetTypesToLoad;
// 	for (auto AssetData : AssetDatas)
// 	{
// 		auto const Found = PrimaryAssetTypesToAlwaysLoad.FindByPredicate(
// 			[&AssetData](FPrimaryAssetType const AssetType)
// 			{
// 				FString const AssetClassPathName = AssetData.AssetClassPath.ToString();
// 				UE_LOG(LogCTRLAssets, Warning, TEXT("%s %s"), *AssetClassPathName, *AssetType.ToString());
// 				return AssetClassPathName == AssetType.ToString();
// 			}
// 		);
// 		if (!Found) continue;
// 		FPrimaryAssetType const PrimaryAssetType = *Found;
// 		if (PrimaryAssetType.IsValid())
// 		{
// 			NewPrimaryAssetTypesToLoad.Add(PrimaryAssetType);
// 		}
// 	}
// 	return MoveTemp(NewPrimaryAssetTypesToLoad);
// }
//
// void UCTRLAssetsSettings::OnAssetsAdded(TArrayView<FAssetData const> const AssetDatas)
// {
// 	UE_LOG(LogCTRLAssets, Warning, TEXT("OnAssetsAdded %d"), AssetDatas.Num());
// 	TArray<FPrimaryAssetType> NewPrimaryAssetTypesToLoad = FindRelevantPrimaryAssetTypes(AssetDatas);
// 	UE_LOG(LogCTRLAssets, Warning, TEXT("\tOnAssetsAdded NewPrimaryAssetTypesToLoad %d"), NewPrimaryAssetTypesToLoad.Num());
// 	if (!NewPrimaryAssetTypesToLoad.Num()) return;
//
// 	TArray<TSharedPtr<FStreamableHandle>> NewHandles;
// 	NewHandles.Reserve(NewPrimaryAssetTypesToLoad.Num());
// 	for (auto const PrimaryAssetType : NewPrimaryAssetTypesToLoad)
// 	{
// 		auto Handle = UAssetManager::Get().LoadPrimaryAssetsWithType(PrimaryAssetType);
// 		if (!Handle.IsValid()) continue;
// 		AllLoadingHandles.Add(Handle); // add to all list so it stays loaded
// 		NewHandles.Add(Handle); // track new handles, so we can wait for only the new handles
// 	}
// 	
// 	UE_LOG(LogCTRLAssets, Warning, TEXT("\tOnAssetsAdded NewHandles %d"), NewHandles.Num());
// 	// wait for assets to load
// 	for (auto const Handle : NewHandles)
// 	{
// 		Handle->WaitUntilComplete();
// 	}
// }
//
// void UCTRLAssetsSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
// {
// 	Super::PostEditChangeProperty(PropertyChangedEvent);
// 	Load();
// }
//
// void UCTRLAssetsSettings::Register()
// {
// 	OnInitHandle = FCoreDelegates::OnPostEngineInit.AddUObject(this, &ThisClass::OnPostEngineInit);
// 	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
// 	AssetRegistry.OnAssetsAdded().AddUObject(this, &ThisClass::OnAssetsAdded);
// }
//
// void UCTRLAssetsSettings::Unregister()
// {
// 	FCoreDelegates::OnPostEngineInit.Remove(OnInitHandle);
// 	AllLoadingHandles.Empty();
// }

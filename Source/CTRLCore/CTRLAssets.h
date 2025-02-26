// // Fill out your copyright notice in the Description page of Project Settings.
//
// #pragma once
//
// #include "CoreMinimal.h"
//
// #include "Engine/DeveloperSettings.h"
//
// #include "CTRLAssets.generated.h"
//
// struct FStreamableHandle;
// /**
//  * 
//  */
// UCLASS(DefaultConfig, Config="Game", meta=(DisplayName="CTRL.Assets"))
// class CTRLCORE_API UCTRLAssetsSettings : public UDeveloperSettings
// {
// 	GENERATED_BODY()
// protected:
// 	DECLARE_LOG_CATEGORY_CLASS(LogCTRLAssets, Log, All);
// public:
// 	UCTRLAssetsSettings();
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TArray<FPrimaryAssetType> PrimaryAssetTypesToAlwaysLoad;
//
// 	void OnPostEngineInit();
// 	void Load();
// 	void OnAssetsAdded(TArrayView<FAssetData const> AssetDatas);
// 	void Register();
// 	void Unregister();
//
// 	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//
// protected:
// 	FDelegateHandle OnInitHandle;
// 	TArray<TSharedPtr<FStreamableHandle>> AllLoadingHandles;
// 	TArray<FPrimaryAssetType> FindRelevantPrimaryAssetTypes(TArrayView<FAssetData const> AssetDatas) const;
//
// #if WITH_EDITORONLY_DATA
//
// public:
// 	virtual FName GetContainerName() const override { return TEXT("Editor"); }
// 	virtual FName GetCategoryName() const override { return TEXT("CTRL"); }
// 	virtual FText GetSectionText() const override { return FText::FromString(TEXT("Assets")); }
// #endif
// };

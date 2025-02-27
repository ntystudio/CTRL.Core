// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "CTRLAssetThumbnails.generated.h"

class UStaticMesh;

/**
 * Implement this to have a nested static mesh used as the thumbnail. e.g. for a DataAsset with a Mesh property.
 * Recommended: Add a UAssetDefinitionDefault for each implementing class,
 * and give them a custom color so types are more easily differentiatable.
 */
UINTERFACE(BlueprintType)
class CTRLCORE_API UCTRLStaticMeshThumbnailProvider : public UInterface
{
	GENERATED_BODY()
};

class CTRLCORE_API ICTRLStaticMeshThumbnailProvider
{
	GENERATED_BODY()

public:
	/**
	 * Get StaticMesh to use as thumbnail.
	 * Falls back to normal, default thumbnail/icon behaviour if returns nullptr/invalid.
	 */
	virtual UStaticMesh* GetThumbnailMesh() const { return nullptr; }
};

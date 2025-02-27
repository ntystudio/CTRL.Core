// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"
#include "CTRLAssetThumbnails.generated.h"

UINTERFACE(BlueprintType)
class CTRLCORE_API UCTRLStaticMeshThumbnailProvider : public UInterface
{
	GENERATED_BODY()
};

class CTRLCORE_API ICTRLStaticMeshThumbnailProvider
{
	GENERATED_BODY()

public:
#if WITH_EDITORONLY_DATA
	virtual UStaticMesh* GetThumbnailMesh() const { return nullptr; }
#endif
};


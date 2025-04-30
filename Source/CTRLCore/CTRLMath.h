// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CTRLMath.generated.h"

/**
 * 
 */
UCLASS()
class CTRLCORE_API UCTRLMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	//~ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Weights ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ //

	template <typename T>
	static T NormalizeWeights(T const& Weights);

	template <typename T>
	static void NormalizeWeightsInline(T& Weights);

	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Normalize Weights")
	static TArray<float> K2_NormalizeWeights(TArray<float> const& Weights);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static int32 GetWeightedRandomIndex(TArray<float> const& Weights);

	//~ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Vector ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ //

	// Round Vector to Nearest Integer
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector RoundV(FVector const& Vector);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector FloorV(FVector const& Vector);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FVector CeilV(FVector const& Vector);

	//~ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Rotator ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ //

	// Round Rotator to Nearest Integer
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FRotator RoundR(FRotator const& Rotator);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FRotator FloorR(FRotator const& Rotator);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FRotator CeilR(FRotator const& Rotator);

	//~ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ LookAt ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ //

	// LookAt only on a single axis. By default, only on the XY plane, ignoring Z
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName="Find Look At Rotation 2D", meta=(AutoCreateRefTerm="InAxis", InAxis="(0,0,1)"))
	static FRotator FindLookAtRotation2d(
		UPARAM(DisplayName="Start") FVector const& InStart,
		UPARAM(DisplayName="End") FVector const& InEnd,
		UPARAM(DisplayName="Axis") FVector const& InAxis = FVector::UpVector,
		bool bFlipStartEnd = false
	);
};

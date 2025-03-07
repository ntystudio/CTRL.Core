// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CTRLMath.generated.h"

/**
 * 
 */
UCLASS(Category="CTRL|Math", DisplayName="Math Utils [CTRL]")
class CTRLCORE_API UCTRLMath : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//~ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Weights ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ //

	template <typename T>
	static T NormalizeWeights(T const& Weights);

	template <typename T>
	static void NormalizeWeightsInline(T& Weights);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CTRL|Math", DisplayName = "Normalize Weights [CTRL]", meta=(Keywords="Random Array Float Number"))
	static TArray<float> K2_NormalizeWeights(TArray<float> const& Weights);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CTRL|Math", DisplayName = "Get Weighted Random Index [CTRL]", meta=(Keywords="Random Weighted Array Float Number"))
	static int32 GetWeightedRandomIndex(TArray<float> const& Weights);

	//~ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Vector ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ //

	// Round Vector to Nearest Integer
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CTRL|Math", DisplayName = "Round Vector [CTRL]", meta=(Keywords="Round Location"))
	static FVector RoundV(FVector const& Vector);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CTRL|Math", DisplayName = "Floor Vector [CTRL]", meta=(Keywords="Round Floor Location"))
	static FVector FloorV(FVector const& Vector);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CTRL|Math", DisplayName = "Ceil Vector [CTRL]", meta=(Keywords="Round Ceiling Location"))
	static FVector CeilV(FVector const& Vector);

	//~ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ Rotator ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ //

	// Round Rotator to Nearest Integer
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CTRL|Math", DisplayName = "Round Rotator [CTRL]", meta=(Keywords="Round Rotate"))
	static FRotator RoundR(FRotator const& Rotator);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CTRL|Math", DisplayName = "Floor Rotator [CTRL]", meta=(Keywords="Round Floor Rotate"))
	static FRotator FloorR(FRotator const& Rotator);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="CTRL|Math", DisplayName = "Ceil Rotator [CTRL]", meta=(Keywords="Round Ceiling Rotate"))
	static FRotator CeilR(FRotator const& Rotator);

	//~ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ LookAt ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ //

	// LookAt only on a single axis. By default, only on the XY plane, ignoring Z
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName="Find Look At Rotation 2D [CTRL]", Category="CTRL|Math", meta=(Keywords="LookAt Camera Face Facing Rotate Get Direction", AutoCreateRefTerm="InAxis", InAxis="(0,0,1)"))
	static FRotator FindLookAtRotation2d(
		UPARAM(DisplayName="Start") FVector const& InStart,
		UPARAM(DisplayName="End") FVector const& InEnd,
		UPARAM(DisplayName="Axis") FVector const& InAxis = FVector::UpVector,
		bool bFlipStartEnd = false
	);
};

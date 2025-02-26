// SPDX-FileCopyrightText: 2025 NTY.studio

#include "CTRLCore/CTRLMath.h"

#include "Algo/Accumulate.h"

#include "Kismet/KismetMathLibrary.h"

FVector UCTRLMath::RoundV(FVector const& Vector)
{
	return FVector(FMath::RoundToInt(Vector.X), FMath::RoundToInt(Vector.Y), FMath::RoundToInt(Vector.Z));
}

FRotator UCTRLMath::RoundR(FRotator const& Rotator)
{
	return FRotator(FMath::RoundToInt(Rotator.Pitch), FMath::RoundToInt(Rotator.Roll), FMath::RoundToInt(Rotator.Yaw));
}

FVector UCTRLMath::FloorV(FVector const& Vector)
{
	return FVector(FMath::Floor(Vector.X), FMath::Floor(Vector.Y), FMath::Floor(Vector.Z));
}

FRotator UCTRLMath::FloorR(FRotator const& Rotator)
{
	return FRotator(FMath::Floor(Rotator.Pitch), FMath::Floor(Rotator.Roll), FMath::Floor(Rotator.Yaw));
}

FVector UCTRLMath::CeilV(FVector const& Vector)
{
	return FVector(FMath::Floor(Vector.X), FMath::Floor(Vector.Y), FMath::Floor(Vector.Z));
}

FRotator UCTRLMath::CeilR(FRotator const& Rotator)
{
	return FRotator(FMath::CeilToDouble(Rotator.Pitch), FMath::CeilToDouble(Rotator.Roll), FMath::CeilToDouble(Rotator.Yaw));
}

template <typename T>
T UCTRLMath::NormalizeWeights(T const& Weights)
{
	T NormalizedWeights = Weights;
	NormalizeWeightsInline(NormalizedWeights);
	return MoveTemp(NormalizedWeights);
}

template <typename T>
void UCTRLMath::NormalizeWeightsInline(T& Weights)
{
	using V = typename T::ElementType;
	V TotalWeight = Algo::Accumulate(Weights, 0);
	TotalWeight = !FMath::IsNearlyZero(TotalWeight) ? TotalWeight : 1;
	for (auto& Weight : Weights)
	{
		Weight /= TotalWeight;
	}
}

TArray<float> UCTRLMath::K2_NormalizeWeights(TArray<float> const& Weights)
{
	return NormalizeWeights(Weights);
}

int32 UCTRLMath::GetWeightedRandomIndex(TArray<float> const& Weights)
{
	auto NormalizedWeights = NormalizeWeights(Weights);

	float const RandomValue = FMath::Rand();

	float CumulativeProbability = 0.f;
	for (int32 Index = 0; Index < NormalizedWeights.Num(); ++Index)
	{
		CumulativeProbability += NormalizedWeights[Index];
		if (RandomValue <= CumulativeProbability || Index == NormalizedWeights.Num() - 1)
		{
			return Index;
		}
	}
	return -1;
}

FRotator UCTRLMath::FindLookAtRotation2d(FVector const& InStart, FVector const& InEnd, FVector const& InAxis, bool const bFlipStartEnd)
{
	auto const AxisNullify = (FVector::OneVector - InAxis.GetAbs()).GetSafeNormal();
	auto Start = InStart * AxisNullify;
	auto End = InEnd * AxisNullify;
	if (bFlipStartEnd)
	{
		Swap(Start, End);
	}
	return UKismetMathLibrary::FindLookAtRotation(Start, End);
}

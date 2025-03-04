// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollisionQueryParams.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

#include "UObject/Object.h"

#include "CTRLCollision.generated.h"

UENUM(BlueprintType)
enum class ECTRLDrawDebugType: uint8
{
	Line,
	Arrow,
};

UENUM(BlueprintType)
enum class ECTRLDrawDebugDuration: uint8
{
	SingleFrame,
	Persistent,
	Duration
};

USTRUCT(BlueprintType)
struct CTRLCORE_API FCTRLDrawDebugOptions
{
	GENERATED_BODY()

	static FCTRLDrawDebugOptions const Default;

	FCTRLDrawDebugOptions() = default;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECTRLDrawDebugType Type = ECTRLDrawDebugType::Line;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECTRLDrawDebugDuration Duration = ECTRLDrawDebugDuration::SingleFrame;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FColor Color = FColor::Turquoise;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bPersistentLines = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="-1.0", UIMin="-1.0", Delta="0.1", Units="s", EditCondition="Duration == ECTRLDrawDebugDuration::Duration"))
	float LifeTime = -1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0.0", UIMin="0.0", Delta="1.0", Units="cm"))
	float Thickness = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin="0.0", Delta="1.0"))
	float DepthPriority = SDPG_Foreground;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(UIMin="0.0", Delta="1.0", Units="cm", EditCondition="Type == ECTRLDrawDebugType::Arrow", EditConditionHides))
	float ArrowSize = 25.f;

	EDrawDebugTrace::Type GetDrawDebugType() const
	{
		if (bDrawDebug)
		{
			if (Duration == ECTRLDrawDebugDuration::SingleFrame)
			{
				return EDrawDebugTrace::ForOneFrame;
			}
			else if (Duration == ECTRLDrawDebugDuration::Persistent)
			{
				return EDrawDebugTrace::Persistent;
			}
			else if (Duration == ECTRLDrawDebugDuration::Duration)
			{
				return EDrawDebugTrace::ForDuration;
			}
		}
		return EDrawDebugTrace::None;
	}
	void PostEdit()
	{
		if (Duration == ECTRLDrawDebugDuration::SingleFrame)
		{
			bPersistentLines = false;
			LifeTime = 0.f;
		}
		else if (Duration == ECTRLDrawDebugDuration::Persistent)
		{
			bPersistentLines = true;
			LifeTime = -1.f;
		}
		else if (Duration == ECTRLDrawDebugDuration::Duration)
		{
			bPersistentLines = false;
		}
	}

	static void Draw(UObject const* WorldContext, FVector const& Start, FVector const& End, FCTRLDrawDebugOptions const& Options)
	{
		if (!Options.bDrawDebug) { return; }
		if (Options.Type == ECTRLDrawDebugType::Line)
		{
			DrawLine(WorldContext, Start, End, Options);
		}
		else if (Options.Type == ECTRLDrawDebugType::Arrow)
		{
			DrawArrow(WorldContext, Start, End, Options);
		}
	}

	static FCTRLDrawDebugOptions MakeConsistent(FCTRLDrawDebugOptions const& Options)
	{
		auto CleanOptions = Options;
		CleanOptions.PostEdit();
		return MoveTemp(CleanOptions);
	}

	static void DrawLine(UObject const* WorldContext, FVector const& Start, FVector const& End, FCTRLDrawDebugOptions const& InOptions);

	static void DrawArrow(UObject const* WorldContext, FVector const& Start, FVector const& End, FCTRLDrawDebugOptions const& InOptions);

	FCTRLDrawDebugOptions(FCTRLDrawDebugOptions const& Other)
		: bDrawDebug(Other.bDrawDebug),
		Type(Other.Type),
		Duration(Other.Duration),
		Color(Other.Color),
		bPersistentLines(Other.bPersistentLines),
		LifeTime(Other.LifeTime),
		Thickness(Other.Thickness),
		DepthPriority(Other.DepthPriority),
		ArrowSize(Other.ArrowSize) {}

	FCTRLDrawDebugOptions(FCTRLDrawDebugOptions&& Other) noexcept
		: bDrawDebug(Other.bDrawDebug),
		Type(Other.Type),
		Duration(Other.Duration),
		Color(std::move(Other.Color)),
		bPersistentLines(Other.bPersistentLines),
		LifeTime(Other.LifeTime),
		Thickness(Other.Thickness),
		DepthPriority(Other.DepthPriority),
		ArrowSize(Other.ArrowSize) {}

	FCTRLDrawDebugOptions& operator=(FCTRLDrawDebugOptions const& Other)
	{
		if (this == &Other)
		{
			return *this;
		}
		bDrawDebug = Other.bDrawDebug;
		Type = Other.Type;
		Duration = Other.Duration;
		Color = Other.Color;
		bPersistentLines = Other.bPersistentLines;
		LifeTime = Other.LifeTime;
		Thickness = Other.Thickness;
		DepthPriority = Other.DepthPriority;
		ArrowSize = Other.ArrowSize;
		return *this;
	}

	FCTRLDrawDebugOptions& operator=(FCTRLDrawDebugOptions&& Other) noexcept
	{
		if (this == &Other) { return *this; }
		bDrawDebug = Other.bDrawDebug;
		Type = Other.Type;
		Duration = Other.Duration;
		Color = std::move(Other.Color);
		bPersistentLines = Other.bPersistentLines;
		LifeTime = Other.LifeTime;
		Thickness = Other.Thickness;
		DepthPriority = Other.DepthPriority;
		ArrowSize = Other.ArrowSize;
		return *this;
	}

	friend bool operator==(FCTRLDrawDebugOptions const& Lhs, FCTRLDrawDebugOptions const& RHS)
	{
		return Lhs.bDrawDebug == RHS.bDrawDebug
			&& Lhs.Type == RHS.Type
			&& Lhs.Duration == RHS.Duration
			&& Lhs.Color == RHS.Color
			&& Lhs.bPersistentLines == RHS.bPersistentLines
			&& Lhs.LifeTime == RHS.LifeTime
			&& Lhs.Thickness == RHS.Thickness
			&& Lhs.DepthPriority == RHS.DepthPriority
			&& Lhs.ArrowSize == RHS.ArrowSize;
	}

	friend bool operator!=(FCTRLDrawDebugOptions const& Lhs, FCTRLDrawDebugOptions const& RHS) { return !(Lhs == RHS); }
};

USTRUCT(BlueprintType)
struct CTRLCORE_API FCTRLIgnoreOptions
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<TObjectPtr<AActor>> IgnoredActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<TObjectPtr<UPrimitiveComponent>> IgnoredComponents;
};

USTRUCT(BlueprintType)
struct CTRLCORE_API FCTRLCollisionQueryParams
{
	GENERATED_BODY()

	/** Whether we should trace against complex collision */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bTraceComplex = false;

	/** Whether we want to find out initial overlap or not. If true, it will return if this was initial overlap. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	bool bFindInitialOverlaps = true;

	/** Whether we want to return the triangle face index for complex static mesh traces */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	bool bReturnFaceIndex = false;

	/** Whether we want to include the physical material in the results. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	bool bReturnPhysicalMaterial = false;

	/** Whether to ignore blocking results. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	bool bIgnoreBlocks = false;

	/** Whether to ignore touch/overlap results. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	bool bIgnoreTouches = false;

	/** Whether to skip narrow phase checks (only for overlaps). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	bool bSkipNarrowPhase = false;

	/** Whether to ignore traces to the cluster union and trace against its children instead. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	bool bTraceIntoSubComponents = true;

	/** If bTraceIntoSubComponents is true, whether to replace the hit of the cluster union with its children instead. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	bool bReplaceHitWithSubComponents = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<TEnumAsByte<ECollisionChannel>> CollisionChannels = {ECC_WorldStatic, ECC_WorldDynamic};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", UIMin="0", Delta="50", Units="cm"))
	float TraceDistance = 10000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	FCTRLIgnoreOptions Ignore;

	/** Tag used to provide extra information or filtering for debugging of the trace (e.g. Collision Analyzer) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	FName TraceTag = NAME_None;

	/** Tag used to indicate an owner for this trace */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, AdvancedDisplay)
	FName OwnerTag = NAME_None;

	//~ Conversion ~//

	FCollisionQueryParams ToCollisionQueryParams() const;
	FCollisionObjectQueryParams ToObjectQueryParams() const;
};
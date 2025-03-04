// Fill out your copyright notice in the Description page of Project Settings.

#include "CTRLCore/CTRLCollision.h"

#include "DrawDebugHelpers.h"

#include "Engine/Engine.h"

FCTRLDrawDebugOptions const FCTRLDrawDebugOptions::Default = FCTRLDrawDebugOptions();

void FCTRLDrawDebugOptions::DrawLine(UObject const* WorldContext, FVector const& Start, FVector const& End, FCTRLDrawDebugOptions const& InOptions) {
	if (!InOptions.bDrawDebug) { return; }
	auto const Options = MakeConsistent(InOptions);
	DrawDebugLine(
		GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::Assert),
		Start,
		End,
		Options.Color,
		Options.bPersistentLines,
		Options.LifeTime,
		Options.DepthPriority,
		Options.Thickness
	);
}

void FCTRLDrawDebugOptions::DrawArrow(UObject const* WorldContext, FVector const& Start, FVector const& End, FCTRLDrawDebugOptions const& InOptions) {
	if (!InOptions.bDrawDebug) { return; }
	auto const Options = MakeConsistent(InOptions);
	DrawDebugDirectionalArrow(
		GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::Assert),
		Start,
		End,
		Options.ArrowSize,
		Options.Color,
		Options.bPersistentLines,
		Options.LifeTime,
		Options.DepthPriority,
		Options.Thickness
	);
}
FCollisionQueryParams FCTRLCollisionQueryParams::ToCollisionQueryParams() const
{
	FCollisionQueryParams Params(TraceTag, bTraceComplex);
	Params.bFindInitialOverlaps = bFindInitialOverlaps;
	Params.bReturnFaceIndex = bReturnFaceIndex;
	Params.bReturnPhysicalMaterial = bReturnPhysicalMaterial;
	Params.bIgnoreBlocks = bIgnoreBlocks;
	Params.bIgnoreTouches = bIgnoreTouches;
	Params.bSkipNarrowPhase = bSkipNarrowPhase;
	Params.bTraceIntoSubComponents = bTraceIntoSubComponents;
	Params.bReplaceHitWithSubComponents = bReplaceHitWithSubComponents;

	Params.AddIgnoredActors(Ignore.IgnoredActors.Array());
	Params.AddIgnoredComponents(Ignore.IgnoredComponents.Array());
	return MoveTemp(Params);
}

FCollisionObjectQueryParams FCTRLCollisionQueryParams::ToObjectQueryParams() const
{
	FCollisionObjectQueryParams Params;
	for (auto Channel : CollisionChannels)
	{
		Params.AddObjectTypesToQuery(Channel);
	}
	return MoveTemp(Params);
}
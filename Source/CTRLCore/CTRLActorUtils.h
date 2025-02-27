// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CTRLActorUtils.generated.h"

class ACharacter;
class UActorComponent;
class UInputMappingContext;
class AController;

UENUM(BlueprintType)
enum class ECTRLIsValid : uint8
{
	IsValid,
	IsInvalid
};

/**
 * 
 */
UCLASS()
class CTRLCORE_API UCTRLActorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(
		BlueprintCallable,
		meta = (
			ComponentClass = "/Script/Engine.ActorComponent",
			DeterminesOutputType = "ComponentClass",
			ExpandBoolAsExecs = "IsComponentValid",
			WorldContext = "Actor",
			DefaultToSelf = "Actor"
		)
	)
	static UActorComponent* GetValidComponentByClass(
		AActor* Actor,
		TSubclassOf<UActorComponent> const ComponentClass,
		bool& IsComponentValid
	);

	/**
	 * Returns true if the actor is a player controller or player-controlled pawn.
	 * Useful in event graphs where you want to check whether a player triggered the
	 * event and you don't want to think about whether the actor is a pawn or a controller.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Is Player Actor", meta = (CompactNodeTitle = "IsPlayerActor", WorldContext = "Actor", DefaultToSelf = "Actor"))
	static bool IsPlayer(AActor const* Actor);

	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Get Owner Actor",
		meta = (CompactNodeTitle = "GetOwnerActor", WorldContext = "Target", DefaultToSelf = "Target", DeterminesOutputType = "OwnerClass", ExpandEnumAsExecs = "OutIsValid")
	)
	static AActor* GetOwnerActor(UObject* Target, TSubclassOf<AActor> OwnerClass, ECTRLIsValid& OutIsValid);

	template <typename T = AController>
	static T* FindController(UObject const* Target, bool bWalkOuter = true);

	template <typename T = APawn>
	static T* FindPawn(UObject const* Target, bool bWalkOuter = true);

	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Find Pawn",
		meta = (CompactNodeTitle = "FindPawn", WorldContext = "Target", DefaultToSelf = "Target", DeterminesOutputType = "PawnClass", ExpandEnumAsExecs = "OutIsValid")
	)
	static APawn* FindPawn_K2(ECTRLIsValid& OutIsValid, UObject const* Target, TSubclassOf<APawn> PawnClass = nullptr);

	/**
	 * Tries to find a controller for the actor.
	 * If the actor is a pawn, it will return the pawn's controller.
	 * If the actor is a controller, it will return the controller.
	 */
	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Find Controller",
		meta = (WorldContext = "Target", DefaultToSelf = "Target", CompactNodeTitle = "FindController", DeterminesOutputType = "ControllerClass", ExpandEnumAsExecs = "OutIsValid")
	)
	static AController* FindController_K2(ECTRLIsValid& OutIsValid, UObject const* Target, TSubclassOf<AController> ControllerClass = nullptr);

	/**
	 * Returns the local player controller for the given world context object.
	 * Use this instead of GetFirstLocalPlayerController() because that
	 * will return the first player controller in the world, which may not be the
	 * one you want.
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		meta = (CompactNodeTitle = "LocalPlayerController", WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
		Category = "Local Player"
	)
	static APlayerController* GetLocalPlayerController(UObject const* WorldContextObject);

	template <typename T = APawn>
	static T* GetLocalPlayerPawn(UObject const* WorldContextObject);

	template <typename T = ACharacter>
	static T* GetLocalPlayerCharacter(UObject const* WorldContextObject);

	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		DisplayName = "Get Local Player Pawn",
		Category = "Local Player",
		meta = (CompactNodeTitle = "LocalPlayerPawn", WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	)
	static APawn* GetLocalPlayerPawn_K2(UObject const* WorldContextObject);
};

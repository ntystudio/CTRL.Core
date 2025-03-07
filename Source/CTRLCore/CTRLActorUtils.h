// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CTRLActorUtils.generated.h"

class APlayerCameraManager;
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
UCLASS(Category="CTRL|Actor", DisplayName="Actor Utils [CTRL]")
class CTRLCORE_API UCTRLActorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Get Valid Component By Class [CTRL]",
		Category = "CTRL|Actor",
		meta = (
			Keywords = "Find Attached Scene Child Children",
			ComponentClass = "/Script/Engine.ActorComponent",
			DeterminesOutputType = "ComponentClass",
			ExpandBoolAsExecs = "IsComponentValid",
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
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		DisplayName = "Is Player Actor [CTRL]",
		Category = "CTRL|Actor",
		meta = (Keywords="Controller Collision Hit Outer", WorldContext = "Actor", DefaultToSelf = "Actor")
	)
	static bool IsPlayer(AActor const* Actor);

	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Get Owner Actor [CTRL]",
		Category = "CTRL|Actor",
		meta = (Keywords="Find Owning Outer", WorldContext = "Target", DefaultToSelf = "Target", DeterminesOutputType = "OwnerClass", ExpandEnumAsExecs = "OutIsValid")
	)
	static AActor* K2_GetOwnerActor(UObject* Target, TSubclassOf<AActor> OwnerClass, ECTRLIsValid& OutIsValid);
	template <typename T = AActor>
	static T* GetOwnerActor(UObject* Target);

	template <typename T = AController>
	static T* FindController(UObject const* Target, bool bWalkOuter = true);

	template <typename T = APawn>
	static T* FindPawn(UObject const* Target, bool bWalkOuter = true);

	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Find Pawn [CTRL]",
		Category = "CTRL|Actor",
		meta = (Keywords="Get Controller Player Outer AI", DefaultToSelf = "Target", DeterminesOutputType = "PawnClass", ExpandEnumAsExecs = "OutIsValid")
	)
	static APawn* K2_FindPawn(ECTRLIsValid& OutIsValid, UObject const* Target, TSubclassOf<APawn> PawnClass = nullptr);

	/**
	 * Tries to find a controller for the actor.
	 * If the actor is a pawn, it will return the pawn's controller.
	 * If the actor is a controller, it will return the controller.
	 */
	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Find Controller [CTRL]",
		Category = "CTRL|Actor",
		meta = (Keywords="Get Pawn Player AI Outer", DefaultToSelf = "Target", DeterminesOutputType = "ControllerClass", ExpandEnumAsExecs = "OutIsValid", HideSelfPin=false)
	)
	static AController* K2_FindController(ECTRLIsValid& OutIsValid, UObject const* Target, TSubclassOf<AController> ControllerClass = nullptr);

	/**
	 * Returns the local player controller for the given world context object.
	 * Use this instead of GetFirstLocalPlayerController() because that
	 * will return the first player controller in the world, which may not be the
	 * one you want.
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		DisplayName = "Get Local Player Controller [CTRL]",
		Category = "CTRL|Actor",
		meta = (Keywords="Find Pawn", WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	)
	static APlayerController* GetLocalPlayerController(UObject const* WorldContextObject);

	template <typename T = APawn>
	static T* GetLocalPlayerPawn(UObject const* WorldContextObject);

	template <typename T = ACharacter>
	static T* GetLocalPlayerCharacter(UObject const* WorldContextObject);

	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		DisplayName = "Get Local Player Pawn [CTRL]",
		Category = "CTRL|Actor",
		meta = (Keywords="Find Controller", WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	)
	static APawn* K2_GetLocalPlayerPawn(UObject const* WorldContextObject);

	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		DisplayName = "Get Player Camera Manager [CTRL]",
		Category = "CTRL|Actor",
		meta = (
			Keywords="Find",
			WorldContext = "Actor",
			DefaultToSelf = "Actor",
			CameraManagerClass = "/Script/Engine.PlayerCameraManager",
			DeterminesOutputType = "CameraManagerClass",
			ExpandBoolAsExecs = "IsValid"
		)
	)
	static APlayerCameraManager* K2_GetPlayerCameraManager(AActor* Actor, TSubclassOf<APlayerCameraManager> CameraManagerClass, bool& IsValid);

	template <typename T = APlayerCameraManager>
	static APlayerCameraManager* GetPlayerCameraManager(AActor* Actor);

	// UFUNCTION(
	// 	BlueprintCallable,
	// 	DisplayName = "Camera LookAt [CTRL]",
	// 	Category = "CTRL|Actor",
	// 	meta = (Keywords="Controller Player Pawn", WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	// )
	// static void CameraLookAt(AActor* TargetActor, AActor* LookAtActor, float InterpSpeed = 5.0f);
};

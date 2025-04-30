// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#pragma once

#include "Components/SplineComponent.h"

#include "GameFramework/Actor.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CTRLActorUtils.generated.h"

class APlayerCameraManager;
class AAIController;
class ULocalPlayer;
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
UCLASS()
class CTRLCORE_API UCTRLActorUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Get Valid Component By Class [CTRL]",
		meta = (
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

	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Get Valid Component By Name [CTRL]",
		Category = "CTRL|Actor",
		meta = (
			Keywords = "Find Attached Scene Child Children",
			ComponentClass = "/Script/Engine.ActorComponent",
			DeterminesOutputType = "ComponentClass",
			ExpandBoolAsExecs = "IsComponentValid",
			DefaultToSelf = "Actor"
		)
	)
	static UActorComponent* GetValidComponentByName(
		AActor* Actor,
		FName ComponentName,
		TSubclassOf<UActorComponent> const ComponentClass,
		bool& IsComponentValid
	);

	/**
	 * Returns true if the actor is a player controller or player-controlled pawn.
	 * Useful in event graphs where you want to check whether a player triggered the
	 * event and you don't want to think about whether the actor is a pawn or a controller.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "Is Player Actor [CTRL]", meta = (WorldContext = "Actor", DefaultToSelf = "Actor"))
	static bool IsPlayer(AActor const* Actor);

	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Get Owner Actor [CTRL]",
		meta = (WorldContext = "Target", DefaultToSelf = "Target", DeterminesOutputType = "OwnerClass", ExpandEnumAsExecs = "OutIsValid")
	)
	static AActor* K2_GetOwnerActor(UObject* Target, TSubclassOf<AActor> OwnerClass, ECTRLIsValid& OutIsValid);

	template <typename T = AActor>
	static T* GetOwnerActor(UObject* Target);

	static AActor* GetOwnerActor(UObject* Target, TSubclassOf<AActor> OwnerClass, ECTRLIsValid& OutIsValid);
	template <typename T = AActor>
	static T* GetOwnerActor(UObject const* Target);

	// Get Actor from Target. If Target is an Actor, return it. If Target is a component, return the owner actor.
	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Get Actor [CTRL]",
		Category = "CTRL|Actor",
		meta = (Keywords="Find Owning Outer", WorldContext = "Target", DefaultToSelf = "Target", DeterminesOutputType = "ActorClass", ExpandEnumAsExecs = "OutIsValid")
	)
	static AActor* K2_GetActor(UObject* Target, TSubclassOf<AActor> ActorClass, ECTRLIsValid& OutIsValid);

	template <typename T = AActor>
	static T* GetActor(UObject const* Target);

	template <typename T = AController>
	static T* FindController(UObject const* Target, bool bWalkOuter = true);

	template <typename T = APawn>
	static T* FindPawn(UObject const* Target, bool bWalkOuter = true);

	/**
	 * Returns the pawn for the given target object.
	 * Will search the outer chain for a controller or pawn and return the first pawn found.
	 * @param Target The target object to find the pawn for.
	 * @param PawnClass The class of the pawn to find. If null, will return any pawn.
	 * @return The pawn for the given target object, or nullptr if none exists.
	 */
	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Find Pawn [CTRL]",
		meta = (DefaultToSelf = "Target", DeterminesOutputType = "PawnClass", ExpandEnumAsExecs = "OutIsValid")
	)
	static APawn* FindPawn_K2(ECTRLIsValid& OutIsValid, UObject const* Target, TSubclassOf<APawn> PawnClass = nullptr);

	/**
	 * Returns the controller for the given target object.
	 * Will search the outer chain for a controller or pawn and return the first controller found.
	 * @param Target The target object to find the controller for.
	 * @param ControllerClass The class of the controller to find. If null, will return any controller.
	 * @return The controller for the given target object, or nullptr if none exists.
	 */
	UFUNCTION(
		BlueprintCallable,
		DisplayName = "Find Controller [CTRL]",
		meta = (DefaultToSelf = "Target", DeterminesOutputType = "ControllerClass", ExpandEnumAsExecs = "OutIsValid", HideSelfPin=false)
	)
	static AController* FindController_K2(ECTRLIsValid& OutIsValid, UObject const* Target, TSubclassOf<AController> ControllerClass = nullptr);

	/**
	 * Returns the local player for the given world context object.
	 * On the server, this may not exist, but on the client, it will always be the player.
	 * @param WorldContextObject 
	 * @return The local player for the given world context object, or nullptr if none exists.
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		DisplayName = "Get Local Player [CTRL]",
		Category = "CTRL|Actor",
		meta = (Keywords="Find Pawn Controller", WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	)
	static ULocalPlayer* GetLocalPlayer(UObject const* WorldContextObject);


	/**
	 * Returns the local player controller for the given world context object.
	 * On the server, this may not exist, but on the client, it will always be the player controller.
	 * @param WorldContextObject 
	 * @return The local player controller for the given world context object, or nullptr if none exists.
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		DisplayName = "Get Local Player Controller [CTRL]",
		meta = (WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject"),
		Category = "Local Player"
	)
	static APlayerController* GetLocalPlayerController(UObject const* WorldContextObject);

	template <typename T = APawn>
	static T* GetLocalPlayerPawn(UObject const* WorldContextObject);

	template <typename T = ACharacter>
	static T* GetLocalPlayerCharacter(UObject const* WorldContextObject);

	/**
	 * Returns the local player pawn for the given world context object.
	 * On the server, this may not exist, but on the client, it will always be the player pawn.
	 * @param WorldContextObject 
	 * @return The local player pawn for the given world context object, or nullptr if none exists.
	 */
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		DisplayName = "Get Local Player Pawn [CTRL]",
		Category = "Local Player",
		meta = (WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	)
	static APawn* K2_GetLocalPlayerPawn(UObject const* WorldContextObject);

	template <typename T = APlayerCameraManager>
	static APlayerCameraManager* GetPlayerCameraManager(AActor* Actor);

	// UFUNCTION(
	// 	BlueprintCallable,
	// 	DisplayName = "Camera LookAt [CTRL]",
	// 	Category = "CTRL|Actor",
	// 	meta = (Keywords="Controller Player Pawn", WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	// )
	// static void CameraLookAt(AActor* TargetActor, AActor* LookAtActor, float InterpSpeed = 5.0f);

	/**
	 * Returns the player camera manager for the given actor.
	 * @param Actor The actor to find the player camera manager for.
	 * @param CameraManagerClass The class of the camera manager to find. If null, will return any camera manager.
	 * @return The player camera manager for the given actor, or nullptr if none exists.
	 */
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

	// UFUNCTION(
	// 	BlueprintCallable,
	// 	DisplayName = "Camera LookAt [CTRL]",
	// 	Category = "CTRL|Actor",
	// 	meta = (Keywords="Controller Player Pawn", WorldContext = "WorldContextObject", DefaultToSelf = "WorldContextObject")
	// )
	// static void CameraLookAt(AActor* TargetActor, AActor* LookAtActor, float InterpSpeed = 5.0f);
};

// explicit template instantiation
template CTRLCORE_API AActor* UCTRLActorUtils::GetOwnerActor<AActor>(UObject const* Target);
template CTRLCORE_API AActor* UCTRLActorUtils::GetActor<AActor>(UObject const* Target);
template CTRLCORE_API AAIController* UCTRLActorUtils::FindController<AAIController>(UObject const* Target, bool bWalkOuter);
template CTRLCORE_API APlayerController* UCTRLActorUtils::FindController<APlayerController>(UObject const* Target, bool bWalkOuter);
template CTRLCORE_API AController* UCTRLActorUtils::FindController<AController>(UObject const* Target, bool bWalkOuter);
template CTRLCORE_API APawn* UCTRLActorUtils::FindPawn<APawn>(UObject const* Target, bool bWalkOuter);

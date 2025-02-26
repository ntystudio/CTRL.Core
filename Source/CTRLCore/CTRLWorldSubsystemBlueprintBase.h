// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Editor.h"

#include "Engine/AssetManager.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Engine/World.h"

#include "Subsystems/EngineSubsystem.h"
#include "Subsystems/WorldSubsystem.h"

#include "CTRLWorldSubsystemBlueprintBase.generated.h"

/**
 * Specifies the goal/source of a UWorld object
 * Blueprint-visible version of EWorldType
 */
UENUM(BlueprintType)
enum class ECTRLWorldType : uint8
{
	/** An untyped world, in most cases this will be the vestigial worlds of streamed in sublevels */
	None,

	/** The game world */
	Game,

	/** A world being edited in the editor */
	Editor,

	/** A Play In Editor world */
	PIE,

	/** A preview world for an editor tool */
	EditorPreview,

	/** A preview world for a game */
	GamePreview,

	/** A minimal RPC world for a game */
	GameRPC,

	/** An editor world that was loaded but not being edited in the level editor */
	Inactive
};

/**
 * Subclass this to implement a world subsystem in Blueprint.
 * 
 * You need to add an entry for CTRLWorldSubsystemBlueprintBase into: "Project Settings" → "Asset Manager" → "Primary Asset Types to Scan"
 * Without this UE won't always load the BP subsystems.
 * 
 * Remember to return any other UWorldSubsystem dependencies in an InitializeDependencies override,
 * this ensures these subsystems are initialized before this subsystem.
 */
UCLASS(BlueprintType, Blueprintable, Abstract, DisplayName = "World Subsystem Blueprint Base [CTRL]")
class CTRLCORE_API UCTRLWorldSubsystemBlueprintBase : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UCTRLWorldSubsystemBlueprintBase();

	/**
	 * Returns a pointer to the UWorld this subsystem is contained within.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure, DisplayName = "GetWorld")
	UWorld* K2_GetWorld() const;

	/**
	 * Implement this for deinitialization of instances of the system.
	 * Called when world is destroyed/changed.
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnDeinitialize")
	void K2_Deinitialize();
	virtual void Deinitialize() override;

	/**
	 * Override this to initialize any dependencies of this Subsystem.
	 * This ensures subsystem dependencies are initialized before this Subsystem.
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "InitializeDependencies")
	TArray<TSubclassOf<USubsystem>> K2_InitializeDependencies();

	/**
	 * Implement this for deinitialization of instances of the system.
	 * Called when world is destroyed/changed.
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnInitialize")
	void K2_Initialize();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Called once all UWorldSubsystems have been initialized */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "PostInitialize")
	void K2_PostInitialize();
	virtual void PostInitialize() override;

	/**
	 * Override to control if the Subsystem should be created at all.
	 * For example, you could only have your system created on servers.
	 * It's important to note that if using this, it becomes important to null check whenever getting the Subsystem.
	 *
	 * Note: This function is called on the CDO prior to instances being created!
	 * You will need to restart the editor to see changes to this function.
	 */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "ShouldCreateSubsystem")
	bool K2_ShouldCreateSubsystem(UWorld* World) const;
	virtual bool K2_ShouldCreateSubsystem_Implementation(UWorld* World) const;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/**
	 * Called when world is ready to start gameplay before the game mode transitions to the correct state and call BeginPlay on all actors
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnWorldBeginPlay")
	void K2_OnWorldBeginPlay(UWorld* InWorld);
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	/**
	 * Called after world components (e.g. line batcher and all level components) have been updated
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnWorldComponentsUpdated")
	void K2_OnWorldComponentsUpdated(UWorld* World);
	virtual void OnWorldComponentsUpdated(UWorld& World) override;

	/**
	 * Override to control if the Subsystem should be created for the given world type.
	 * @note You will need to restart the editor to see changes to this function.
	 */
	UFUNCTION(BlueprintNativeEvent, DisplayName = "DoesSupportWorldType")
	bool K2_DoesSupportWorldType(ECTRLWorldType const WorldType) const;
	virtual bool K2_DoesSupportWorldType_Implementation(ECTRLWorldType const WorldType) const;
	virtual bool DoesSupportWorldType(EWorldType::Type const WorldType) const override;

protected:
	bool bHasBPShouldCreateSubsystem = false;
	bool bHasBPDoesSupportWorldType = false;
};

/**
 * This ensures any BP-subclasses of UCTRLWorldSubsystemBlueprintBase are loaded at editor/game startup and before entering PIE.
 * Make sure to configure an entry for CTRLWorldSubsystemBlueprintBase in "Project Settings" → "Asset Manager" → "Primary Asset Types to Scan"
 */
UCLASS(NotBlueprintType)
class CTRLCORE_API UCTRLWorldSubsystemBlueprintLoader : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	void LoadAssets();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

protected:
	TSharedPtr<FStreamableHandle> LoadingHandle;

	void OnInitialScanComplete();
	void PostEngineInit();
	void PreBeginPIE(bool bIsSimulating);
};

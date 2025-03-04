// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#include "CTRLWorldSubsystemBlueprintBase.h"

#include "CTRLCore/CTRLObject.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CTRLWorldSubsystemBlueprintBase)

UCTRLWorldSubsystemBlueprintBase::UCTRLWorldSubsystemBlueprintBase()
{
	bHasBPShouldCreateSubsystem = CTRL::Object::IsImplementedInBlueprint(this, GET_FUNCTION_NAME_CHECKED(ThisClass, K2_ShouldCreateSubsystem));
	bHasBPDoesSupportWorldType = CTRL::Object::IsImplementedInBlueprint(this, GET_FUNCTION_NAME_CHECKED(ThisClass, K2_DoesSupportWorldType));
}

UWorld* UCTRLWorldSubsystemBlueprintBase::K2_GetWorld() const
{
	return GetWorld();
}

void UCTRLWorldSubsystemBlueprintBase::Deinitialize()
{
	Super::Deinitialize();
	K2_Deinitialize();
}

void UCTRLWorldSubsystemBlueprintBase::Initialize(FSubsystemCollectionBase& Collection)
{
	for (auto const& SubsystemClass : K2_InitializeDependencies())
	{
		Collection.InitializeDependency(SubsystemClass);
	}
	Super::Initialize(Collection);
	K2_Initialize();
}

void UCTRLWorldSubsystemBlueprintBase::PostInitialize()
{
	Super::PostInitialize();
	K2_PostInitialize();
}

bool UCTRLWorldSubsystemBlueprintBase::K2_ShouldCreateSubsystem_Implementation(UWorld* World) const
{
	if (!Super::ShouldCreateSubsystem(World)) return false;
	if (!DoesSupportWorldType(World->WorldType)) return false;
	return true;
}

bool UCTRLWorldSubsystemBlueprintBase::ShouldCreateSubsystem(UObject* Outer) const
{
	if (GetClass()->HasAnyClassFlags(CLASS_Abstract)) return false; // Abstract subsystem, should never be created.
	if (!bHasBPShouldCreateSubsystem) return Super::ShouldCreateSubsystem(Outer);
	return K2_ShouldCreateSubsystem(CastChecked<UWorld>(Outer));
}

void UCTRLWorldSubsystemBlueprintBase::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	K2_OnWorldBeginPlay(&InWorld);
}

void UCTRLWorldSubsystemBlueprintBase::OnWorldComponentsUpdated(UWorld& World)
{
	Super::OnWorldComponentsUpdated(World);
	K2_OnWorldComponentsUpdated(&World);
}

bool UCTRLWorldSubsystemBlueprintBase::K2_DoesSupportWorldType_Implementation(ECTRLWorldType const WorldType) const
{
	return Super::DoesSupportWorldType(static_cast<EWorldType::Type>(WorldType));
}

bool UCTRLWorldSubsystemBlueprintBase::DoesSupportWorldType(EWorldType::Type const WorldType) const
{
	if (!bHasBPDoesSupportWorldType) return Super::DoesSupportWorldType(WorldType);
	return K2_DoesSupportWorldType(static_cast<ECTRLWorldType>(WorldType));
}

void UCTRLWorldSubsystemBlueprintLoader::Initialize(FSubsystemCollectionBase& Collection)
{
	AssetTypes.Add(UCTRLWorldSubsystemBlueprintBase::StaticClass()->GetFName());
	Super::Initialize(Collection);
}
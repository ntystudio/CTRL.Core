// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#include "CTRLActorUtils.h"

#include "AIController.h"

#include "Components/ActorComponent.h"

#include "Engine/Engine.h"

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CTRLActorUtils)

UActorComponent* UCTRLActorUtils::GetValidComponentByClass(AActor* Actor, TSubclassOf<UActorComponent> const ComponentClass, bool& IsComponentValid)
{
	IsComponentValid = false;
	if (!IsValid(Actor)) { return nullptr; }
	if (auto const Component = Actor->FindComponentByClass(ComponentClass))
	{
		IsComponentValid = true;
		return Component;
	}
	return nullptr;
}

bool UCTRLActorUtils::IsPlayer(AActor const* Actor)
{
	if (!IsValid(Actor)) { return false; }
	if (auto const Pawn = Cast<APawn>(Actor))
	{
		return Pawn->IsPlayerControlled();
	}

	if (auto const Controller = Cast<AController>(Actor))
	{
		return Controller->IsPlayerController();
	}

	return false;
}

AActor* UCTRLActorUtils::GetOwnerActor(UObject* Target, TSubclassOf<AActor> OwnerClass, ECTRLIsValid& OutIsValid)
{
	OutIsValid = ECTRLIsValid::IsInvalid;
	if (!IsValid(Target)) { return nullptr; }
	if (!OwnerClass)
	{
		OwnerClass = AActor::StaticClass();
	}

	if (!IsValid(OwnerClass)) return nullptr;
	AActor* OwnerActor = nullptr;
	ON_SCOPE_EXIT
	{
		OutIsValid = IsValid(OwnerActor) ? ECTRLIsValid::IsValid : ECTRLIsValid::IsInvalid;
	};

	if (auto const Component = Cast<UActorComponent>(Target))
	{
		if (auto const Owner = Component->GetOwner())
		{
			if (Owner->IsA(OwnerClass))
			{
				OwnerActor = Owner;
				return OwnerActor;
			}
		}
		return nullptr;
	}
	if (auto const Actor = Cast<AActor>(Target))
	{
		if (auto const Owner = Actor->GetOwner())
		{
			if (Owner->IsA(OwnerClass))
			{
				OwnerActor = Owner;
				return OwnerActor;
			}
		}
		return nullptr;
	}

	OwnerActor = Cast<AActor>(Target->GetTypedOuter(OwnerClass));
	return OwnerActor;
}

template <typename T>
T* UCTRLActorUtils::FindPawn(UObject const* Target, bool const bWalkOuter)
{
	if (auto const* Pawn = Cast<T>(Target))
	{
		return const_cast<APawn*>(Pawn);
	}

	if (auto const Controller = FindController(Target, bWalkOuter))
	{
		return Controller->GetPawn<T>();
	}

	if (!bWalkOuter) { return nullptr; }
	return Target->GetTypedOuter<T>();
}

APawn* UCTRLActorUtils::FindPawn_K2(ECTRLIsValid& OutIsValid, UObject const* Target, TSubclassOf<APawn> PawnClass)
{
	OutIsValid = ECTRLIsValid::IsInvalid;
	if (PawnClass == nullptr)
	{
		PawnClass = APawn::StaticClass();
	}
	if (!IsValid(Target) || !IsValid(PawnClass)) { return nullptr; }
	if (auto const Pawn = FindPawn(Target))
	{
		if (Pawn->IsA(PawnClass))
		{
			OutIsValid = ECTRLIsValid::IsValid;
			return Pawn;
		}
	}
	return nullptr;
}

template <typename T>
T* UCTRLActorUtils::FindController(UObject const* Target, bool const bWalkOuter)
{
	if (!IsValid(Target)) { return nullptr; }
	// Check directly for controller
	if (auto const* Controller = Cast<T>(Target))
	{
		return const_cast<T*>(Controller);
	}
	// Check for Pawn, use Pawn's controller
	if (auto const Pawn = Target->IsA<APawn>() ? Cast<APawn>(Target) : bWalkOuter ? Target->GetTypedOuter<APawn>() : nullptr)
	{
		if (auto const Controller = Pawn->GetController<T>())
		{
			return Controller;
		}
		return nullptr;
	}

	if (auto const Actor = Cast<AActor>(Target))
	{
		// Check for instigating controller
		if (auto const InstigatingController = Actor->GetInstigatorController<T>())
		{
			return InstigatingController;
		}
	}

	if (auto const Component = Cast<UActorComponent>(Target))
	{
		return FindController<T>(Component->GetOwner(), bWalkOuter);
	}
	if (!bWalkOuter) { return nullptr; }
	// Check for outer controller
	return Target->GetTypedOuter<T>();
}

// Explicit template instantiation
template AAIController* UCTRLActorUtils::FindController<AAIController>(UObject const* Target, bool bWalkOuter);
template APlayerController* UCTRLActorUtils::FindController<APlayerController>(UObject const* Target, bool bWalkOuter);

AController* UCTRLActorUtils::FindController_K2(ECTRLIsValid& OutIsValid, UObject const* Target, TSubclassOf<AController> ControllerClass)
{
	OutIsValid = ECTRLIsValid::IsInvalid;

	if (ControllerClass == nullptr)
	{
		ControllerClass = AController::StaticClass();
	}

	if (!IsValid(Target) || !IsValid(ControllerClass)) { return nullptr; }

	if (auto const Controller = FindController<AController>(Target))
	{
		if (Controller->IsA(ControllerClass))
		{
			OutIsValid = ECTRLIsValid::IsValid;
			return Controller;
		}
	}

	return nullptr;
}

APlayerController* UCTRLActorUtils::GetLocalPlayerController(UObject const* WorldContextObject)
{
	check(WorldContextObject);
	auto const World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	return GEngine->GetFirstLocalPlayerController(World);
}

APawn* UCTRLActorUtils::GetLocalPlayerPawn_K2(UObject const* WorldContextObject)
{
	return GetLocalPlayerPawn<APawn>(WorldContextObject);
}

template <typename T>
T* UCTRLActorUtils::GetLocalPlayerPawn(UObject const* WorldContextObject)
{
	if (auto const PC = GetLocalPlayerController(WorldContextObject))
	{
		return PC->GetPawn<T>();
	}
	return nullptr;
}

template <typename T>
T* UCTRLActorUtils::GetLocalPlayerCharacter(UObject const* WorldContextObject)
{
	if (auto const PC = GetLocalPlayerController(WorldContextObject))
	{
		return Cast<T>(PC->GetCharacter());
	}
	return nullptr;
}

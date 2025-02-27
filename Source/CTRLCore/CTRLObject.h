// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

#include "Engine/BlueprintGeneratedClass.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "UObject/Interface.h"

#include "CTRLObject.generated.h"

class UStaticMesh;

namespace CTRL::Object
{
	// e.g. bHasBPDoesSupportWorldType = CTRL::Object::IsImplementedInBlueprint(this, GET_FUNCTION_NAME_CHECKED(ThisClass, K2_DoesSupportWorldType));
	bool IsImplementedInBlueprint(UObject const* Object, FName const FuncName);
}

UCLASS()
class UCTRLObject : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};

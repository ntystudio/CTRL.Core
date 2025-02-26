// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Engine/BlueprintGeneratedClass.h"

namespace CTRL::Object
{
	// e.g. bHasBPDoesSupportWorldType = CTRL::Object::IsImplementedInBlueprint(this, GET_FUNCTION_NAME_CHECKED(ThisClass, K2_DoesSupportWorldType));
	static bool IsImplementedInBlueprint(UObject const* Object, FName const FuncName)
	{
		if (!ensure(Object)) return false;
		UFunction const* Func = Object->GetClass()->FindFunctionByName(FuncName);
		if (!ensureMsgf(Func, TEXT("Class for object %s does not have function %s"), *GetNameSafe(Object), *FuncName.ToString()))
		{
			return false;
		}

		return (
			ensure(Func->GetOuter())
			&& Func->GetOuter()->IsA(UBlueprintGeneratedClass::StaticClass())
		);
	}
}

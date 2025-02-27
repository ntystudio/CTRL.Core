// Fill out your copyright notice in the Description page of Project Settings.

#include "CTRLCore/CTRLObject.h"

bool CTRL::Object::IsImplementedInBlueprint(UObject const* Object, FName const FuncName)
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

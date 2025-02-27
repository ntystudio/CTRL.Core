// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#include "CTRLObject.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CTRLObject)

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

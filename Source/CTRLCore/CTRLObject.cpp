// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#include "CTRLObject.h"

#include "Templates/SubclassOf.h"

#include "UObject/UObjectIterator.h"

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

TArray<UClass*> CTRL::Object::GetSubclassesOf(TSubclassOf<UObject> const& Class)
{
	if (!Class)
	{
		return TArray<UClass*>();
	}

	TMap<FString, UClass*> SubclassesMap;

	for (TObjectIterator<UClass> It; It; ++It)
	{
		UClass* Subclass = *It;
		if (Subclass->IsChildOf(Class))
		{
			SubclassesMap.Add(Subclass->GetName(), Subclass);
		}
	}

	// Another loop to remove the unnecessary classes
	TArray<UClass*> Subclasses;

	for (TPair<FString, UClass*> Pair : SubclassesMap)
	{
		// REINST_ classes are not needed
		if (Pair.Key.StartsWith("REINST_") || Pair.Key.StartsWith("SKEL_"))
		{
			FString OriginalClassName = Pair.Key.RightChop(Pair.Key.Find("_") + 1);

			if (SubclassesMap.Contains(OriginalClassName))
			{
				continue;
			}
		}

		Subclasses.Add(Pair.Value);
	}

	return Subclasses;
}

TArray<UClass*> UCTRLObject::GetSubclassesOf(TSubclassOf<UObject> const Class)
{
	return CTRL::Object::GetSubclassesOf(Class);
}

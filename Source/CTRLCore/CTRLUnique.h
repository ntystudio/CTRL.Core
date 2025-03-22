// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CTRLUnique.generated.h"

/**
 * 
 */
UCLASS()
class CTRLCORE_API UCTRLUnique : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// Get a short ID for the given GUID that is unambiguous within the given set of GUIDs.
	// This is similar to git partial hashes, where the first few characters are used to identify a commit.
	// The ID will be at least MinLength characters long, and will be as short as possible while still being unique.
	// e.g. FGuid "11310AED2E554D61AF679AA3C5A1082C" → "11310AED"
	// If the GUID is invalid, an empty string will be returned.
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		Category = "CTRL|Unique",
		meta = (DisplayName = "Get Unambiguous Short ID [CTRL]", Keywords = "GUID, Unique, ID")
	)
	static FString GetUnambiguousShortId(TSet<FGuid> Guids, FGuid const& Guid, int32 const MinLength = 4)
	{
		if (!Guid.IsValid()) return TEXT("");
		if (!Guids.Num()) return Guid.ToString().Left(MinLength);
		int32 MaxCommonLength = MinLength;
		for (auto const& OtherGuid : Guids)
		{
			if (OtherGuid == Guid) continue;
			if (OtherGuid.IsValid())
			{
				FString OtherGuidStr = OtherGuid.ToString();
				int32 CommonLength = 0;
				for (int32 i = 0; i < FMath::Min(OtherGuidStr.Len(), Guid.ToString().Len()); ++i)
				{
					if (OtherGuidStr[i] == Guid.ToString()[i])
					{
						CommonLength++;
					}
					else
					{
						break;
					}
				}
				MaxCommonLength = FMath::Max(MaxCommonLength, CommonLength);
			}
		}
		return Guid.ToString().Left(MaxCommonLength);
	}

};

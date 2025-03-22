// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "CTRLGameplayTags.generated.h"

/**
 * 
 */
UCLASS(Category="CTRL|GameplayTags", DisplayName="GameplayTag Utils [CTRL]")
class CTRLCORE_API UCTRLGameplayTags : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		Category = "CTRL|GameplayTags",
		meta= (AutoCreateRefTerm="Tags", DisplayName = "Make MatchAnyTags Query [CTRL]", Keywords = "GameplayTags, GameplayTag, Match")
	)
	static FGameplayTagQuery MakeQuery_MatchAnyTags(FGameplayTagContainer const& Tags);
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		Category = "CTRL|GameplayTags",
		meta= (AutoCreateRefTerm="Tags", DisplayName = "Make MatchNoTags Query [CTRL]", Keywords = "GameplayTags, GameplayTag, Match")
	)
	static FGameplayTagQuery MakeQuery_MatchNoTags(FGameplayTagContainer const& Tags);
	UFUNCTION(
		BlueprintCallable,
		BlueprintPure,
		Category = "CTRL|GameplayTags",
		meta= (AutoCreateRefTerm="Tags", DisplayName = "Make MatchAllTags Query [CTRL]", Keywords = "GameplayTags, GameplayTag, Match")
	)
	static FGameplayTagQuery MakeQuery_MatchAllTags(FGameplayTagContainer const& Tags);
};

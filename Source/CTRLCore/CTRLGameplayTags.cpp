// Fill out your copyright notice in the Description page of Project Settings.

#include "CTRLGameplayTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CTRLGameplayTags)

FGameplayTagQuery UCTRLGameplayTags::MakeQuery_MatchAnyTags(FGameplayTagContainer const& Tags)
{
	return FGameplayTagQuery::MakeQuery_MatchAnyTags(Tags);
}

FGameplayTagQuery UCTRLGameplayTags::MakeQuery_MatchNoTags(FGameplayTagContainer const& Tags)
{
	return FGameplayTagQuery::MakeQuery_MatchNoTags(Tags);
}

FGameplayTagQuery UCTRLGameplayTags::MakeQuery_MatchAllTags(FGameplayTagContainer const& Tags)
{
	return FGameplayTagQuery::MakeQuery_MatchAllTags(Tags);
}

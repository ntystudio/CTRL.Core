// SPDX-FileCopyrightText: © 2025 NTY.studio
// SPDX-License-Identifier: MIT

#pragma once

#include "CoreMinimal.h"

#include "Engine/DeveloperSettings.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Logging/TokenizedMessage.h"

#include "CTRLMsgLog.generated.h"

class FTextToken;

UENUM(DisplayName="Message Severity [CTRL]", Category="CTRL|MsgLog")
enum class ECTRLMessageSeverity : uint8
{
	CriticalError = 0,
	Error = 1,
	PerformanceWarning = 2,
	Warning = 3,
	Info = 4,
};

/**
 * 
 */
UCLASS(DefaultConfig, Config="Editor", meta=(DisplayName="CTRL.MsgLog"))
class CTRLCORE_API UCTRLMsgLogSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DefaultLogName = "CTRL.Core";

#if WITH_EDITORONLY_DATA
	virtual FName GetContainerName() const override { return TEXT("Editor"); }
	virtual FName GetCategoryName() const override { return TEXT("CTRL"); }
	virtual FText GetSectionText() const override { return FText::FromString(TEXT("Message Log")); }
#endif
};

/**
 * 
 */
UCLASS(Category="CTRL|Message Log", DisplayName="Message Log [CTRL]")
class CTRLCORE_API UCTRLMsgLog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Log message to the MessageLog + object token, based on a condition.
	// Returns true if the condition is met.
	UFUNCTION(BlueprintCallable, Category="CTRL|Message Log", meta = (Keywords="Debug Check Log Msg Print String Text", WorldContext = "Subject", DefaultToSelf = "Subject"))
	static bool EnsureLog(bool bCondition, UObject const* Subject, FString const& Message = "", ECTRLMessageSeverity Severity = ECTRLMessageSeverity::Warning, bool bOpenMsgLog = true);

	// Log a message in the MessageLog at specified severity.
	static void Log(FString const& Message, ECTRLMessageSeverity Severity = ECTRLMessageSeverity::Warning, bool bOpenMsgLog = true);

	// Log a message in the MessageLog with a reference to the passed-in object at specified severity.
	UFUNCTION(BlueprintCallable, Category="CTRL|Message Log", meta = (Keywords="Debug Log Msg Print String Text", WorldContext = "Subject", DefaultToSelf = "Subject"))
	static void Log(UObject const* Subject, FString const& Message, ECTRLMessageSeverity Severity = ECTRLMessageSeverity::Warning, bool bOpenMsgLog = true);

	// add message to log or console log
	static void AddMessage(TSharedRef<FTokenizedMessage> const& Msg, bool bOpenMsgLog = true);

	// Print a message to the output log
	FORCEINLINE static void OutputLog(TSharedRef<FTokenizedMessage> const& Msg, ECTRLMessageSeverity Severity = ECTRLMessageSeverity::Warning);

	// Simplified arguments for Actor token creation.
	static TSharedRef<IMessageToken> ActorToken(AActor const* Actor, FString const& InMessage = "");

	// Create appropriate token for object. e.g. ActorToken for Actors, UObjectToken for UObjects, etc. Fill find actor for components.
	static TSharedRef<IMessageToken> Token(UObject const* Object, FString const& Message = "");

	// Simple text token
	static TSharedRef<FTextToken> Token(FString const& String);

	// Simple text token
	static TSharedRef<FTextToken> TextToken(FString const& String);

	// Create a TokenizedMessage for an object with a message and severity.
	static TSharedRef<FTokenizedMessage> ObjectMessage(UObject const* Subject, FString const& Message, ECTRLMessageSeverity Severity = ECTRLMessageSeverity::Warning);

	// Create a TokenizedMessage for an object with a message and severity.
	static TSharedRef<FTokenizedMessage> StringMessage(FString const& Message, ECTRLMessageSeverity const Severity = ECTRLMessageSeverity::Warning);

	// Get short path to object for logging.
	UFUNCTION(BlueprintCallable, Category="CTRL|Message Log", BlueprintPure, meta=(Keywords="Find String Object Package", WorldContext = "Object", DefaultToSelf = "Object"))
	static FString GetShortPath(UObject const* Object);

	// Convert CTRL Severity to Engine MsgLog Severity.
	static EMessageSeverity::Type GetSeverity(ECTRLMessageSeverity Severity);
};

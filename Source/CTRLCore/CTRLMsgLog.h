// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Kismet/BlueprintFunctionLibrary.h"

#include "Logging/TokenizedMessage.h"

#include "CTRLMsgLog.generated.h"

class FTextToken;

USTRUCT(BlueprintType)
struct FCTRLMsgLogSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DefaultLogName = "CTRL.Core";
};

UENUM()
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
UCLASS()
class CTRLCORE_API UCTRLMsgLog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Log message to the MessageLog + object token, based on a condition.
	// Returns true if the condition is met.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "Subject", DefaultToSelf = "Subject"))
	static bool EnsureLog(bool bCondition, UObject const* Subject, FString const& Message = "", ECTRLMessageSeverity Severity = ECTRLMessageSeverity::Warning);
	
	// Log a message in the MessageLog at specified severity.
	static void Log(FString const& Message, ECTRLMessageSeverity Severity = ECTRLMessageSeverity::Warning);

	// Log a message in the MessageLog with a reference to the passed-in object at specified severity.
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "Subject", DefaultToSelf = "Subject"))
	static void Log(UObject const* Subject, FString const& Message, ECTRLMessageSeverity Severity = ECTRLMessageSeverity::Warning);

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
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static FString GetShortPath(UObject const* Object);

	// Print the message to the output log.
	FORCEINLINE static void OutputLog(TSharedRef<FTokenizedMessage> const& Msg, ECTRLMessageSeverity Severity);

	// Convert CTRL Severity to Engine MsgLog Severity.
	static EMessageSeverity::Type GetSeverity(ECTRLMessageSeverity Severity);
};

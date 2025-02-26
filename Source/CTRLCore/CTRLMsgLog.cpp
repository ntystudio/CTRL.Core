// Fill out your copyright notice in the Description page of Project Settings.

#include "CTRLCore/CTRLMsgLog.h"

#include "AssetRegistry/AssetDataToken.h"

#include "CTRLCore/CTRLCore.h"

#include "GameFramework/Actor.h"

#include "Logging/MessageLog.h"

//~ ───────────────────────────────── Logging ───────────────────────────────── //

bool UCTRLMsgLog::EnsureLog(bool const bCondition, UObject const* Subject, FString const& Message, ECTRLMessageSeverity const Severity, bool const bOpenMsgLog)
{
	if (bCondition) { return true; }
	Log(Subject, Message, Severity, bOpenMsgLog);
	return false;
}

void UCTRLMsgLog::Log(UObject const* Subject, FString const& Message, ECTRLMessageSeverity const Severity, bool const bOpenMsgLog)
{
	AddMessage(ObjectMessage(Subject, Message, Severity), bOpenMsgLog);
}

void UCTRLMsgLog::Log(FString const& Message, ECTRLMessageSeverity const Severity, bool const bOpenMsgLog)
{
	AddMessage(StringMessage(Message, Severity), bOpenMsgLog);
}


//~ ───────────────────────────────────── Output ───────────────────────────────────── //

void UCTRLMsgLog::AddMessage(TSharedRef<FTokenizedMessage> const& Msg, bool const bOpenMsgLog)
{
#if WITH_EDITOR
	auto const Settings = GetDefault<UCTRLMsgLogSettings>();
	FMessageLog MessageLog(Settings->DefaultLogName);
	MessageLog.AddMessage(Msg);
	if (bOpenMsgLog) MessageLog.Open();
#else
	OutputLog(Msg, Severity);
#endif
}

// ReSharper disable once CyclomaticComplexity
void UCTRLMsgLog::OutputLog(TSharedRef<FTokenizedMessage> const& Msg, ECTRLMessageSeverity const Severity)
{
	switch (Severity)
	{
		case ECTRLMessageSeverity::CriticalError:
			UE_LOG(LogCTRLCore, Error, TEXT("CriticalError: %s"), *Msg->ToText().ToString());
			break;
		case ECTRLMessageSeverity::Error:
			UE_LOG(LogCTRLCore, Error, TEXT("%s"), *Msg->ToText().ToString());
			break;
		case ECTRLMessageSeverity::PerformanceWarning:
			UE_LOG(LogCTRLCore, Warning, TEXT("PerformanceWarning: %s"), *Msg->ToText().ToString());
			break;
		case ECTRLMessageSeverity::Warning:
			UE_LOG(LogCTRLCore, Warning, TEXT("%s"), *Msg->ToText().ToString());
			break;
		case ECTRLMessageSeverity::Info:
			UE_LOG(LogCTRLCore, Log, TEXT("%s"), *Msg->ToText().ToString());
			break;
	}
}

//~ ───────────────────────────────── Message Tokens ───────────────────────────────── //

TSharedRef<IMessageToken> UCTRLMsgLog::Token(UObject const* Object, FString const& Message)
{
	FString const MsgString = Message.IsEmpty() ? Message : GetNameSafe(Object); //FString::Printf(TEXT("%s"), *GetShortPath(Object));
	return FAssetDataToken::Create(Object, FText::FromString(MsgString));
}

TSharedRef<FTextToken> UCTRLMsgLog::Token(FString const& String)
{
	return TextToken(String);
}

TSharedRef<FTextToken> UCTRLMsgLog::TextToken(FString const& String)
{
	return FTextToken::Create(FText::FromString(String));
}

TSharedRef<IMessageToken> UCTRLMsgLog::ActorToken(AActor const* Actor, FString const& InMessage)
{
	if (!IsValid(Actor) || !Actor->IsValidLowLevel()) { return TextToken(FString::Printf(TEXT("Invalid Actor: %s"), *InMessage)); }
	auto const Message = FString::Printf(TEXT("%32s"), *(InMessage.IsEmpty() ? Actor->GetActorNameOrLabel() : InMessage));
	return FAssetDataToken::Create(Actor, FText::FromString(Message));
}

TSharedRef<FTokenizedMessage> UCTRLMsgLog::StringMessage(FString const& Message, ECTRLMessageSeverity const Severity)
{
	return FTokenizedMessage::Create(GetSeverity(Severity), FText::FromString(Message));
}

TSharedRef<FTokenizedMessage> UCTRLMsgLog::ObjectMessage(UObject const* Subject, FString const& Message, ECTRLMessageSeverity const Severity)
{
	if (!IsValid(Subject))
	{
		return FTokenizedMessage::Create(GetSeverity(Severity))
			->AddText(FText::FromString(TEXT("Invalid Object")))
			->AddText(FText::FromString(Message));
	}

	return FTokenizedMessage::Create(GetSeverity(Severity), FText::FromString(Message))
		->AddToken(Token(Subject, *GetNameSafe(Subject)))
		->AddText(FText::FromString(FString::Printf(TEXT(" [%s]"), *GetShortPath(Subject))));
}

//~ ───────────────────────────────────── Utils ────────────────────────────────────── //

FString UCTRLMsgLog::GetShortPath(UObject const* Object)
{
	if (!IsValid(Object)) { return TEXT("nullptr"); }
	return Object->GetPathName(Object->GetTypedOuter<AActor>());
}

// Convert from ECTRLMessageSeverity to EMessageSeverity
EMessageSeverity::Type UCTRLMsgLog::GetSeverity(ECTRLMessageSeverity const Severity)
{
	switch (Severity)
	{
		case ECTRLMessageSeverity::CriticalError:
		{
			PRAGMA_DISABLE_DEPRECATION_WARNINGS
			// ReSharper disable once CppDeprecatedEntity
			return EMessageSeverity::CriticalError;
			PRAGMA_ENABLE_DEPRECATION_WARNINGS
		}
		case ECTRLMessageSeverity::Error:
			return EMessageSeverity::Error;
		case ECTRLMessageSeverity::PerformanceWarning:
			return EMessageSeverity::PerformanceWarning;
		case ECTRLMessageSeverity::Warning:
			return EMessageSeverity::Warning;
		case ECTRLMessageSeverity::Info:
			return EMessageSeverity::Info;
		default:
		{
			checkNoEntry();
			return EMessageSeverity::Error;
		};
	};
}

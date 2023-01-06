// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#define CUR_FUNC_NAME (FString(__FUNCTION__))
#define CUR_LINE  (FString::FromInt(__LINE__))

#define CUR_LOG_POSITION (CUR_FUNC_NAME + "(" + CUR_LINE + ")")

//Print Log
#define SALogInfo(LogCat, OutStr)	UE_LOG(LogCat, Log, TEXT("%s"), *FString(OutStr))
#define SALogWarn(LogCat, OutStr)	UE_LOG(LogCat, Warning, TEXT("%s"), *FString(OutStr))
#define SALogError(LogCat, OutStr)	UE_LOG(LogCat, Error, TEXT("%s"), *FString(OutStr))

//Print Message On Screen
#define SAScreenLog(Param1) (SAScreenLogT(Param1, 5))
#define SAScreenLogT(Param1, Param2)	(GEngine->AddOnScreenDebugMessage(-1, Param2, FColor::White, *( ": " + Param1)))
#define SAScreenWarn(Param1) (SAScreenWarnT(Param1, 5))
#define SAScreenWarnT(Param1, Param2)	(GEngine->AddOnScreenDebugMessage(-1, Param2, FColor::Yellow, *( ": " + Param1)))
#define SAScreenError(Param1) (SAScreenErrorT(Param1, 5))
#define SAScreenErrorT(Param1, Param2)	(GEngine->AddOnScreenDebugMessage(-1, Param2, FColor::Red, *(": " + Param1)))

static bool m_Enable;

class FSALog
{
public:
	
	static void Warning(const FString CurLogPosition, const FString& LogStr);
	static void Warning(const FString CurLogPosition, const int& LogStr);
	static void Error(const FString CurLogPosition, const FString& LogStr);
	static void Error(const FString CurLogPosition, const int& LogStr);
	static void Info(const FString CurLogPosition, const FString& LogStr);
	static void Info(const FString CurLogPosition, const int& LogStr);
	static void SetEnableLog(bool Enable);
};


// Copyright 2022 SensorsData. All Rights Reserved.
#include "SALog.h"

DEFINE_LOG_CATEGORY_STATIC(SA_PC, Log, All);

void FSALog::Warning(const FString CurLogPosition, const FString& LogStr)
{
    if (m_Enable)
    {
        SALogWarn(SA_PC, FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
        SAScreenWarn(FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
    }  
}

void FSALog::Warning(const FString CurLogPosition, const int& LogStr)
{
    if (m_Enable)
    {
        SALogWarn(SA_PC, FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
        SAScreenWarn(FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
    }       
}

void FSALog::Error(const FString CurLogPosition, const FString& LogStr)
{
    if (m_Enable)
    {
        SALogError(SA_PC, FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
        SAScreenLog(FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
  	}
}

void FSALog::Error(const FString CurLogPosition, const int& LogStr)
{
    if (m_Enable)
    {
        SALogError(SA_PC, FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
        SAScreenLog(FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
    }   
}

void FSALog::Info(const FString CurLogPosition, const FString& LogStr)
{
    if (m_Enable)
    {
        SALogInfo(SA_PC, FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
        SAScreenLog(FString::Printf(TEXT("%s --- %s"), *CurLogPosition, *LogStr));
    }
}

void FSALog::Info(const FString CurLogPosition, const int& LogStr)
{
    if (m_Enable)
    {
        SALogInfo(SA_PC, FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
        SAScreenLog(FString::Printf(TEXT("%s --- %d"), *CurLogPosition, LogStr));
    }   
}

void FSALog::SetEnableLog(bool Enable)
{
   	m_Enable = Enable;	
}    

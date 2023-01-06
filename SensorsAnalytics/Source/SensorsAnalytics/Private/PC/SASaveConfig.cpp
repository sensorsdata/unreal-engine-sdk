// Copyright 2022 SensorsData. All Rights Reserved.
#include "SASaveConfig.h"


USASaveConfig::USASaveConfig()
{
    UserIndex = FSAConstants::SA_SLOT_USER_INDEX_CONFIG;
}

void USASaveConfig::SetDistinctID(const FString& DistinctID)
{
    this->m_DistinctID = DistinctID;
}

void USASaveConfig::SetAnonymousID(const FString& AnonymousID)
{
    this->m_AnonymousID = AnonymousID;
}

void USASaveConfig::SetLoginId(const FString& LoginId)
{
    this->m_LoginId = LoginId;
}

void USASaveConfig::SetDeviceID(const FString& DeviceID)
{
    this->m_DeviceID = DeviceID;
}

void USASaveConfig::SetSerialID(const FString& SerialID)
{
    this->m_SerialID = SerialID;
}

void USASaveConfig::SetUUID(const FString& UUID)
{
    this->m_UUID = UUID;
}

void USASaveConfig::SetFirstDayTime(const FString& FirstDayTime)
{
    this->m_FirstDayTime = FirstDayTime;
}

void USASaveConfig::SetSuperProperties(const FString& SuperProperties)
{
    this->m_SuperProperties = SuperProperties;
}

void USASaveConfig::SetAppInstall(bool bIsAppInstall)
{
    this->m_bIsAppInstall = bIsAppInstall;
}

void USASaveConfig::AddAll(USASaveConfig *SaveConfig)
{
    if ( SaveConfig != nullptr )
    {
        SetDistinctID(SaveConfig->m_DistinctID);
        SetAnonymousID(SaveConfig->m_AnonymousID);
        SetLoginId(SaveConfig->m_LoginId);
        SetDeviceID(SaveConfig->m_DeviceID);
        SetSerialID(SaveConfig->m_SerialID);
        SetUUID(SaveConfig->m_UUID);
        SetFirstDayTime(SaveConfig->m_FirstDayTime);
        SetSuperProperties(SaveConfig->m_SuperProperties);
        SetAppInstall(SaveConfig->m_bIsAppInstall);
    }
    else
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Passing a nullptr USASaveConfig !"));
    }
}

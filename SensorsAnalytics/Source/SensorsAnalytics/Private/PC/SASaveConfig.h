// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#include "../Common/SALog.h"
#include "../Common/SAConstants.h"

#include "GameFramework/SaveGame.h"
#include "SASaveConfig.generated.h"

UCLASS()
class USASaveConfig : public USaveGame
{
    GENERATED_BODY()
public:
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_DistinctID;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_AnonymousID;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_LoginId;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_DeviceID;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_SerialID;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_UUID;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_FirstDayTime;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString m_SuperProperties;
    
    UPROPERTY(VisibleAnywhere, Category = Basic)
    bool m_bIsAppInstall;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;

	USASaveConfig();

    void SetDistinctID(const FString& DistinctID);
    
    void SetAnonymousID(const FString& AnonymousID);

    void SetLoginId(const FString& LoginId);
    
    void SetDeviceID(const FString& DeviceID);
    
    void SetSerialID(const FString& SerialID);

    void SetUUID(const FString& UUID);
    
    void SetFirstDayTime(const FString& FirstDayTime);

    void SetSuperProperties(const FString& SuperProperties);
    
    void SetAppInstall(bool bIsAppInstall);

    void AddAll(USASaveConfig *SaveConfig);
};

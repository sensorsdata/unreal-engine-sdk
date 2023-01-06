// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#include "SASaveConfig.h"
#include "../Common/SAUtils.h"
#include "../Common/SALog.h"
#include "../Common/SAConstants.h"
#include "SARequestHelper.h"
#include "SAEventManager.h"
#include "SensorsAnalyticsSettings.h"

#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Kismet/GameplayStatics.h"

#include "SensorsAnalyticsPCBase.generated.h"

UCLASS()
class USensorsAnalyticsPCBase : public UObject
{

GENERATED_UCLASS_BODY()
    
public:
    
    virtual void StartWithConfigOptions(const USensorsAnalyticsSettings * Settings);

    virtual void RegisterSuperProperties(const FString& Properties);

    virtual void UnregisterSuperProperty(const FString& Key);
    
    virtual void ClearSuperProperties();

    virtual void Login(const FString& LoginId);

    virtual void Logout();

    virtual void Track(const FString& EventName, const FString& Properties);
    
    virtual void TrackAppInstall(const FString& Properties, bool bDisableCallback);

    virtual void SetOnce(const FString& Properties);

    virtual void Set(const FString& Properties);
    
    virtual void Unset(const FString& Key);

    virtual void Increment(const FString& Properties);

    virtual void Append(const FString& Key, const FString& Properties);

    virtual void DeleteUser();

    virtual void Flush();

    virtual void DeleteAll();
    
    virtual bool IsEnableSDK();
    
    void SetEventManager(USAEventManager * InputEventManager);
    
    FString SlotName;
    
    FString sa_GetDeviceID();
    
    FString sa_GetSerialID();
    
    FString sa_GetLoginID();

    FString sa_GetDistinctID();

    FString sa_GetAnonymousID();
    
    FString sa_GetUUID();
    
    FString sa_GetFirstDayTime();

    FString sa_GetSuperProperties();

    FString sa_GetPresetProperties();
    
    FString sa_GetLibProperties();
    
    FString sa_GetIdentitiesProperties();
    
    FString sa_GetServerUrl();
        
    float sa_GetFlushInterval();
    
    int sa_GetFlushBulkSize();
    
    ~USensorsAnalyticsPCBase();

private:
    
    // destructor of the base class not private
//    ~USensorsAnalyticsPCBase();
    
    USASaveConfig* m_SaveConfig;

    USAEventManager* m_EventManager;

    FString m_ServerUrl;

    float m_FlushInterval;

    int m_FlushBulkSize;
    
    bool m_bIsAppInstall;

    FString m_LoginId;

    FString m_UUID;

    FString m_DistinctID;

    FString m_AnonymousID;

    FString m_DeviceID;
    
    FString m_SerialID;
    
    FString m_FirstDayTime;
    
    FString m_SuperProperties;

    FString m_PresetProperties;

    FString m_LibProperties;
    
    FString m_IdentitiesProperties;
    
    void Init(const FString& ServerUrl, int FlushInterval, int FlushBulkSize);
    
    void InitIdentitiesProperties();

    void InitLibProperties();

    void InitPresetProperties();
        
    USASaveConfig* ReadValue();

    void SaveValue(USASaveConfig *SaveConfig);
    
};

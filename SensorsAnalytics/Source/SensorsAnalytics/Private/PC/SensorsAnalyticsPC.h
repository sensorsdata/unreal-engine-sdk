// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#include "SensorsAnalyticsPCBase.h"
#include "SensorsAnalyticsPCDisable.h"

class SensorsAnalyticsPC
{

public:
    
    static void StartWithConfigOptions(const USensorsAnalyticsSettings * Settings);

    static void EnableSDK();

    static void DisableSDK();

    static void RegisterSuperProperties(const FString& Properties);

    static void UnregisterSuperProperty(const FString& Key);
    
    static void ClearSuperProperties();

    static void Login(const FString& LoginId);

    static void Logout();

    static void Track(const FString& EventName, const FString& Properties);
    
    static void TrackAppInstall(const FString& Properties, bool bDisableCallback);

    static void SetOnce(const FString& Properties);

    static void Set(const FString& Properties);
    
    static void Unset(const FString& Key);

    static void Increment(const FString& Properties);

    static void Append(const FString& Key, const FString& Properties);

    static void DeleteUser();

    static void Flush();

    static void DeleteAll();
    
private:
    
    static USensorsAnalyticsPCBase* GetInstance();
    
    static void SetEnableLog(bool Enable);
};	

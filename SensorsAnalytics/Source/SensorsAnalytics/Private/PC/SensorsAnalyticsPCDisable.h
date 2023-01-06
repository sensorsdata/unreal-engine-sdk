// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#include "SensorsAnalyticsPCBase.h"

#include "SensorsAnalyticsPCDisable.generated.h"

UCLASS()
class USensorsAnalyticsPCDisable : public USensorsAnalyticsPCBase
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
    
private:
    
    ~USensorsAnalyticsPCDisable();
};

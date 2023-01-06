// Copyright 2022 SensorsData. All Rights Reserved.
#include "SensorsAnalyticsPCDisable.h"

USensorsAnalyticsPCDisable::USensorsAnalyticsPCDisable(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

USensorsAnalyticsPCDisable::~USensorsAnalyticsPCDisable()
{
    
}

void USensorsAnalyticsPCDisable::StartWithConfigOptions(const USensorsAnalyticsSettings * Settings)
{
    this->AddToRoot();
}

void USensorsAnalyticsPCDisable::RegisterSuperProperties(const FString& Properties)
{
    
}

void USensorsAnalyticsPCDisable::UnregisterSuperProperty(const FString& Key)
{
    
}

void USensorsAnalyticsPCDisable::ClearSuperProperties()
{
    
}

void USensorsAnalyticsPCDisable::Login(const FString& LoginId)
{
    
}

void USensorsAnalyticsPCDisable::Logout()
{
    
}

void USensorsAnalyticsPCDisable::Track(const FString& EventName, const FString& Properties)
{
    
}

void USensorsAnalyticsPCDisable::TrackAppInstall(const FString& Properties, bool bDisableCallback)
{
    
}

void USensorsAnalyticsPCDisable::SetOnce(const FString& Properties)
{
    
}

void USensorsAnalyticsPCDisable::Set(const FString& Properties)
{
    
}

void USensorsAnalyticsPCDisable::Unset(const FString& Key)
{
    
}

void USensorsAnalyticsPCDisable::Increment(const FString& Properties)
{
    
}

void USensorsAnalyticsPCDisable::Append(const FString& Key, const FString& Properties)
{
    
}

void USensorsAnalyticsPCDisable::DeleteUser()
{
    
}

void USensorsAnalyticsPCDisable::Flush()
{
    
}

void USensorsAnalyticsPCDisable::DeleteAll()
{
    
}

bool USensorsAnalyticsPCDisable::IsEnableSDK()
{
    return false;
}

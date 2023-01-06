//
//  SensorsAnalytics.cpp
//  FirstGame
//
//  Created by MC on 2022/11/23.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#include "SensorsAnalytics.h"

#include <string>
#include "Interfaces/IPluginManager.h"
#include "ISettingsModule.h"
#include "SensorsAnalyticsSettings.h"
#include "Core.h"
#include "Json.h"
#include "iOS/SensorsAnalyticsIOS.h"
#include "PC/SensorsAnalyticsPC.h"

#if PLATFORM_ANDROID
#define SensorsAnalyticsPlatform SensorsAnalyticsAndroid
#elif PLATFORM_IOS
#define SensorsAnalyticsPlatform SensorsAnalyticsIOS
#else
#define SensorsAnalyticsPlatform SensorsAnalyticsPC
#endif

DEFINE_LOG_CATEGORY_STATIC(LogUSensorsAnalytics, Display, All);

USensorsAnalytics::USensorsAnalytics(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer) 
{
}


void USensorsAnalytics::Start() 
{
    auto &manager = IPluginManager::Get();
    auto plugin = manager.FindPlugin("SensorsAnalytics");

    if (!plugin.IsValid()) 
    {
        return;
    }
    const USensorsAnalyticsSettings *defaultSettings = GetDefault<USensorsAnalyticsSettings>();
    SensorsAnalyticsPlatform::StartWithConfigOptions(defaultSettings);
}

void USensorsAnalytics::StartWithConfigOptions(const USensorsAnalyticsSettings * Settings) 
{
    SensorsAnalyticsPlatform::StartWithConfigOptions(Settings);
}

void USensorsAnalytics::EnableSDK() 
{
    SensorsAnalyticsPlatform::EnableSDK();
}

void USensorsAnalytics::DisableSDK() 
{
    SensorsAnalyticsPlatform::DisableSDK();
}

void USensorsAnalytics::RegisterSuperProperties(const FString& Properties) 
{
    SensorsAnalyticsPlatform::RegisterSuperProperties(Properties);
}

void USensorsAnalytics::UnregisterSuperProperty(const FString& Key) 
{
    SensorsAnalyticsPlatform::UnregisterSuperProperty(Key);
}

void USensorsAnalytics::ClearSuperProperties() 
{
    SensorsAnalyticsPlatform::ClearSuperProperties();
}

void USensorsAnalytics::Login(const FString& LoginId) 
{
    SensorsAnalyticsPlatform::Login(LoginId);
}

void USensorsAnalytics::Logout() 
{
    SensorsAnalyticsPlatform::Logout();
}

void USensorsAnalytics::Track(const FString& EventName, const FString& Properties) 
{
    SensorsAnalyticsPlatform::Track(EventName, Properties);
}

void USensorsAnalytics::TrackAppInstall(const FString& Properties, bool bDisableCallback)
{
    SensorsAnalyticsPlatform::TrackAppInstall(Properties, bDisableCallback);
}

void USensorsAnalytics::SetOnce(const FString& Properties) 
{
    SensorsAnalyticsPlatform::SetOnce(Properties);
}

void USensorsAnalytics::Set(const FString& Properties) 
{
    SensorsAnalyticsPlatform::Set(Properties);
}

void USensorsAnalytics::Increment(const FString& Properties)
{
    SensorsAnalyticsPlatform::Increment(Properties);
}

void USensorsAnalytics::Append(const FString& Key, const FString& Properties)
{
    SensorsAnalyticsPlatform::Append(Key, Properties);
}

void USensorsAnalytics::Unset(const FString& Key)
{
    SensorsAnalyticsPlatform::Unset(Key);
}

void USensorsAnalytics::DeleteUser() 
{
    SensorsAnalyticsPlatform::DeleteUser();
}

void USensorsAnalytics::Flush() 
{
    SensorsAnalyticsPlatform::Flush();
}

void USensorsAnalytics::DeleteAll() 
{
    SensorsAnalyticsPlatform::DeleteAll();
}

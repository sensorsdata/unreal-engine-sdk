//
//  SensorsAnalyticsIOS.h
//  FirstGame
//
//  Created by MC on 2022/11/24.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#if PLATFORM_IOS

#pragma once

#include <string>
#include "SensorsAnalyticsSettings.h"
#include "Serialization/JsonSerializer.h"

class SensorsAnalyticsIOS
{

public:

    static void StartWithConfigOptions(const USensorsAnalyticsSettings * Settings);

    static void EnableSDK();

    static void DisableSDK();

    static void RegisterSuperProperties(const FString& Properties);

    static void UnregisterSuperProperty(const FString& Key);

    static void Login(const FString& LoginId);

    static void Logout();

    static void Track(const FString& EventName, const FString& Properties);

    static void TrackAppInstall(const FString& Properties, bool bDisableCallback);

    static void SetOnce(const FString& Properties);

    static void Set(const FString& Properties);

    static void Increment(const FString& Properties);

    static void Append(const FString& Key, const FString& Properties);

    static void Unset(const FString& Key);

    static void DeleteUser();

    static void ClearSuperProperties();

    static void Flush();

    static void DeleteAll();
};

#endif

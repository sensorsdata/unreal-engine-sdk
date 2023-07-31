//
//  SensorsAnalyticsIOS.cpp
//  FirstGame
//
//  Created by MC on 2022/11/24.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#if PLATFORM_IOS

#include "SensorsAnalyticsIOS.h"
#import <SensorsAnalyticsSDK/SensorsAnalyticsSDK.h>
#include "../Common/SAUtils.h"

static bool bIsAddPluginVersion = false;

void sa_convertToDictionary(const char *json, NSDictionary **dic)
{
    NSString *jsonString = json != NULL ? [NSString stringWithUTF8String:json] : nil;
    if ( jsonString)
    {
        *dic = [NSJSONSerialization JSONObjectWithData:[jsonString dataUsingEncoding:NSUTF8StringEncoding] options:0 error:nil];
    }
}

void sa_convertToArray(const char *json, NSArray **array)
{
    NSString *jsonString = json != NULL ? [NSString stringWithUTF8String:json] : nil;
    if (jsonString)
    {
        *array = [NSJSONSerialization JSONObjectWithData:[jsonString dataUsingEncoding:NSUTF8StringEncoding] options:0 error:nil];
    }
}

void SensorsAnalyticsIOS::StartWithConfigOptions(const USensorsAnalyticsSettings * Settings)
{
    NSString *serverUrl = [NSString stringWithUTF8String:TCHAR_TO_UTF8(*Settings->ServerUrl)];
    dispatch_block_t block = ^{
        SAConfigOptions *options = [[SAConfigOptions alloc] initWithServerURL:serverUrl launchOptions:nil];
        options.flushNetworkPolicy = Settings->FlushNetworkPolicy;
        options.flushInterval = Settings->FlushInterval;
        options.flushBulkSize = Settings->FlushBulkSize;
        options.enableJavaScriptBridge = YES;
        options.enableLog = Settings->bEnableLog;
        options.autoTrackEventType = Settings->AutoTrackEventType;
        [SensorsAnalyticsSDK startWithConfigOptions:options];
    };
    if ([NSThread isMainThread]) {
        block();
    } else {
        dispatch_sync(dispatch_get_main_queue(), block);
    }
}

void SensorsAnalyticsIOS::EnableSDK()
{
    [SensorsAnalyticsSDK enableSDK];
}

void SensorsAnalyticsIOS::DisableSDK()
{
    [SensorsAnalyticsSDK disableSDK];
}

void SensorsAnalyticsIOS::RegisterSuperProperties(const FString& Properties)
{
    NSDictionary *properties = nil;
    sa_convertToDictionary(TCHAR_TO_UTF8(*Properties), &properties);
    [[SensorsAnalyticsSDK sharedInstance] registerSuperProperties:properties];
}

void SensorsAnalyticsIOS::UnregisterSuperProperty(const FString& Key)
{
    NSString *property = (*Key != nullptr && !Key.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*Key)] : nil;
    [[SensorsAnalyticsSDK sharedInstance] unregisterSuperProperty:property];
}

void SensorsAnalyticsIOS::Login(const FString& LoginId)
{
    NSString *loginID = (*LoginId != nullptr && !LoginId.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*LoginId)] : nil;
    [[SensorsAnalyticsSDK sharedInstance] login:loginID];
}

void SensorsAnalyticsIOS::Logout()
{
    [[SensorsAnalyticsSDK sharedInstance] logout];
}

void SensorsAnalyticsIOS::Track(const FString& EventName, const FString& Properties)
{
    NSString *event = (*EventName != nullptr && !EventName.IsEmpty()) ? [NSString stringWithUTF8String:TCHAR_TO_UTF8(*EventName)] : nil;
    
    NSDictionary *properties = nil;
    if (bIsAddPluginVersion)
    {
        sa_convertToDictionary(TCHAR_TO_UTF8(*Properties), &properties);
    }
    else
    {
        bIsAddPluginVersion = true;
        
        FString VersionProperties = FSAUtils::AddPluginVersion(Properties, FSAConstants::SA_PLUGIN_VERSION_KEY, FSAConstants::SA_PLUGIN_VERSION);
        sa_convertToDictionary(TCHAR_TO_UTF8(*VersionProperties), &properties);
    }
    
    [[SensorsAnalyticsSDK sharedInstance] track:event withProperties:properties];
}

void SensorsAnalyticsIOS::TrackAppInstall(const FString& Properties, bool bDisableCallback)
{
    NSDictionary *properties = nil;
    sa_convertToDictionary(TCHAR_TO_UTF8(*Properties), &properties);
    [[SensorsAnalyticsSDK sharedInstance] trackAppInstallWithProperties:properties disableCallback:bDisableCallback];
}

void SensorsAnalyticsIOS::SetOnce(const FString& Properties)
{
    NSDictionary *properties = nil;
    sa_convertToDictionary(TCHAR_TO_UTF8(*Properties), &properties);
    [[SensorsAnalyticsSDK sharedInstance] setOnce:properties];
}

void SensorsAnalyticsIOS::Set(const FString& Properties)
{
    NSDictionary *properties = nil;
    sa_convertToDictionary(TCHAR_TO_UTF8(*Properties), &properties);
    [[SensorsAnalyticsSDK sharedInstance] set:properties];
}

void SensorsAnalyticsIOS::Increment(const FString& Properties)
{
    NSDictionary *properties = nil;
    sa_convertToDictionary(TCHAR_TO_UTF8(*Properties), &properties);
    [[SensorsAnalyticsSDK sharedInstance] increment:properties];
}

void SensorsAnalyticsIOS::Append(const FString& Key, const FString& Properties)
{
    NSString *key = [NSString stringWithUTF8String:TCHAR_TO_UTF8(*Key)];
    NSArray *properties = nil;
    sa_convertToArray(TCHAR_TO_UTF8(*Properties), &properties);
    [[SensorsAnalyticsSDK sharedInstance] append:key by:properties];
}

void SensorsAnalyticsIOS::Unset(const FString& Key)
{
    NSString *key = [NSString stringWithUTF8String:TCHAR_TO_UTF8(*Key)];
    [[SensorsAnalyticsSDK sharedInstance] unset:key];
}

void SensorsAnalyticsIOS::DeleteUser()
{
    [[SensorsAnalyticsSDK sharedInstance] deleteUser];
}

void SensorsAnalyticsIOS::ClearSuperProperties()
{
    [[SensorsAnalyticsSDK sharedInstance] clearSuperProperties];
}

void SensorsAnalyticsIOS::Flush()
{
    [[SensorsAnalyticsSDK sharedInstance] flush];
}

void SensorsAnalyticsIOS::DeleteAll()
{
    [[SensorsAnalyticsSDK sharedInstance] deleteAll];
}

#endif

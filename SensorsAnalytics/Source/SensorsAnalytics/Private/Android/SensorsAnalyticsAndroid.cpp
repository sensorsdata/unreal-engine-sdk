//
//  SensorsAnalyticsAndroid.cpp
//  Hello
//
//  Created by MC on 2022/12/1.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#if PLATFORM_ANDROID

#include "SensorsAnalyticsAndroid.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#include "../Common/SAUtils.h"

#define SENSORS_ANALYTICS "com/sensorsdata/analytics/android/SensorsAnalyticsAPI"

static bool bIsAddPluginVersion = false;

void SensorsAnalyticsAndroid::StartWithConfigOptions(const USensorsAnalyticsSettings * Settings)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saStart", "(Ljava/lang/String;ZIIII)V", false);
    jstring serverUrl = env->NewStringUTF(TCHAR_TO_UTF8(*Settings->ServerUrl));
    jint flushInterval = Settings->FlushInterval;
    jint flushBulkSize = Settings->FlushBulkSize;
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, serverUrl, Settings->bEnableLog, Settings->FlushNetworkPolicy, flushInterval, flushBulkSize, Settings->AutoTrackEventType);
}

void SensorsAnalyticsAndroid::EnableSDK()
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saEnableSDK", "()V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID);
}

void SensorsAnalyticsAndroid::DisableSDK()
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saDisableSDK", "()V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID);
}

void SensorsAnalyticsAndroid::RegisterSuperProperties(const FString& Properties)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saRegisterSuperProperties", "(Ljava/lang/String;)V", false);
    jstring properties = env->NewStringUTF(TCHAR_TO_UTF8(*Properties));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, properties);
}

void SensorsAnalyticsAndroid::UnregisterSuperProperty(const FString& Key)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saUnregisterSuperProperty", "(Ljava/lang/String;)V", false);
    jstring key = env->NewStringUTF(TCHAR_TO_UTF8(*Key));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, key);
}

void SensorsAnalyticsAndroid::Login(const FString& LoginId)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saLogin", "(Ljava/lang/String;)V", false);
    jstring loginId = env->NewStringUTF(TCHAR_TO_UTF8(*LoginId));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, loginId);
}

void SensorsAnalyticsAndroid::Logout()
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saLogout", "()V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID);
}

void SensorsAnalyticsAndroid::Track(const FString& EventName, const FString& Properties)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saTrack", "(Ljava/lang/String;Ljava/lang/String;)V", false);
    jstring eventName = env->NewStringUTF(TCHAR_TO_UTF8(*EventName));
    
    jstring properties;
    if (bIsAddPluginVersion)
    {
        properties = env->NewStringUTF(TCHAR_TO_UTF8(*Properties));
    }
    else
    {
        bIsAddPluginVersion = true;
        
        FString VersionProperties = FSAUtils::AddPluginVersion(Properties, FSAConstants::SA_ANDROID_PLUGIN_VERSION_KEY, FSAConstants::SA_ANDROID_PLUGIN_VERSION);
        properties = env->NewStringUTF(TCHAR_TO_UTF8(*VersionProperties));
    }
    
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, eventName, properties);
}

void SensorsAnalyticsAndroid::TrackAppInstall(const FString& Properties, bool bDisableCallback)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saTrackAppInstall", "(Ljava/lang/String;Z)V", false);
    jstring properties = env->NewStringUTF(TCHAR_TO_UTF8(*Properties));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, properties, bDisableCallback);
}

void SensorsAnalyticsAndroid::SetOnce(const FString& Properties)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saSetOnce", "(Ljava/lang/String;)V", false);
    jstring properties = env->NewStringUTF(TCHAR_TO_UTF8(*Properties));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, properties);
}

void SensorsAnalyticsAndroid::Set(const FString& Properties)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saSet", "(Ljava/lang/String;)V", false);
    jstring properties = env->NewStringUTF(TCHAR_TO_UTF8(*Properties));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, properties);
}

void SensorsAnalyticsAndroid::Increment(const FString& Properties)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saIncrement", "(Ljava/lang/String;)V", false);
    jstring properties = env->NewStringUTF(TCHAR_TO_UTF8(*Properties));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, properties);
}

void SensorsAnalyticsAndroid::Append(const FString& Key, const FString& Properties)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saAppend", "(Ljava/lang/String;Ljava/lang/String;)V", false);
    jstring key = env->NewStringUTF(TCHAR_TO_UTF8(*Key));
    jstring properties = env->NewStringUTF(TCHAR_TO_UTF8(*Properties));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, key, properties);
}

void SensorsAnalyticsAndroid::Unset(const FString& Key)
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saUnset", "(Ljava/lang/String;)V", false);
    jstring key = env->NewStringUTF(TCHAR_TO_UTF8(*Key));
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID, key);
}

void SensorsAnalyticsAndroid::DeleteUser()
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saDeleteUser", "()V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID);
}

void SensorsAnalyticsAndroid::ClearSuperProperties()
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saClearSuperProperties", "()V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID);
}

void SensorsAnalyticsAndroid::Flush()
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saFlush", "()V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID);
}

void SensorsAnalyticsAndroid::DeleteAll()
{
    JNIEnv* env = FAndroidApplication::GetJavaEnv();
    jmethodID methodID = FJavaWrapper::FindMethod(env, FJavaWrapper::GameActivityClassID, "saDeleteAll", "()V", false);
    FJavaWrapper::CallVoidMethod(env, FJavaWrapper::GameActivityThis, methodID);
}

#endif

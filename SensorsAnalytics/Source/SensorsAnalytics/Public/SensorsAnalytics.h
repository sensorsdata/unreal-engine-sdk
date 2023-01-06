// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include <string>
#include <map>
#include <vector>
#include "Core.h"

#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"
#include "Serialization/JsonSerializer.h"
#include "SensorsAnalyticsSettings.h"
#include "SensorsAnalytics.generated.h"

class ISensorsAnalytics;

UCLASS()
class SENSORSANALYTICS_API USensorsAnalytics : public UObject
{
    GENERATED_UCLASS_BODY()

    static inline ISensorsAnalytics& Get();

public:

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void Start();

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void StartWithConfigOptions(const USensorsAnalyticsSettings * Settings);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void EnableSDK();

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void DisableSDK();

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void RegisterSuperProperties(const FString& Properties);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void UnregisterSuperProperty(const FString& Key);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void Login(const FString& LoginId);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void Logout();

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void Track(const FString& EventName, const FString& Properties = "");

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void TrackAppInstall(const FString& Properties, bool bDisableCallback = false);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void SetOnce(const FString& Properties);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void Set(const FString& Properties);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void Increment(const FString& Properties);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void Append(const FString& Key, const FString& Properties);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void Unset(const FString& Key);

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void DeleteUser();

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void ClearSuperProperties();

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void Flush();

    UFUNCTION(BlueprintCallable, Category = "SensorsAnalytics")
    static void DeleteAll();
};

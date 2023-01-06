// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SensorsAnalyticsSettings.generated.h"

UENUM(Meta = (Bitflags))
enum class ESensorsAnalyticsFlushNetworkPolicy
{
    TYPE_2G = 0,
    TYPE_3G = 1,
    TYPE_4G = 2,
    TYPE_Wifi = 3,
    TYPE_5G = 4
};

UENUM(Meta = (Bitflags))
enum class ESensorsAnalyticsAutoTrackEventType
{
    AppStart = 0,
    AppEnd = 1,
};


UCLASS(config = Engine, defaultconfig)
class USensorsAnalyticsSettings : public UObject
{
	GENERATED_BODY()

public:

    USensorsAnalyticsSettings(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(Config, EditAnywhere, Category = "SensorsAnalytics", meta = (DisplayName = "Server Url"))
    FString ServerUrl;

    UPROPERTY(Config, EditAnywhere, Category = "SensorsAnalytics", meta = (DisplayName = "Enable Log"))
    bool bEnableLog;

    UPROPERTY(Config, EditAnywhere, Category = "SensorsAnalytics", meta = (DisplayName = "Flush Network Policy", Bitmask, BitmaskEnum = "ESensorsAnalyticsFlushNetworkPolicy"))
    int32 FlushNetworkPolicy;

    UPROPERTY(Config, EditAnywhere, Category = "SensorsAnalytics", meta = (DisplayName = "Flush Interval"))
    int32 FlushInterval;

    UPROPERTY(Config, EditAnywhere, Category = "SensorsAnalytics", meta = (DisplayName = "Flush Bulk Size"))
    int32 FlushBulkSize;

    UPROPERTY(Config, EditAnywhere, Category = "SensorsAnalytics", meta = (DisplayName = "Auto Track Event Type", Bitmask, BitmaskEnum = "ESensorsAnalyticsAutoTrackEventType"))
    int32 AutoTrackEventType;
};

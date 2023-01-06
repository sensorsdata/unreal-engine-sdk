// Fill out your copyright notice in the Description page of Project Settings.


#include "SensorsAnalyticsSettings.h"

USensorsAnalyticsSettings::USensorsAnalyticsSettings(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer), ServerUrl(""), bEnableLog(false), FlushNetworkPolicy(30), FlushInterval(15000), FlushBulkSize(100)
{
}

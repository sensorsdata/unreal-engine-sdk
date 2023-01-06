// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IAnalyticsProviderModule.h"

class IAnalyticsProvider;

class FSensorsAnalyticsModule : public IAnalyticsProviderModule
{
    TSharedPtr<IAnalyticsProvider> Provider;
public:

    static inline FSensorsAnalyticsModule& Get()
    {
        return FModuleManager::LoadModuleChecked<FSensorsAnalyticsModule>("SensorsAnalytics");
    }

    virtual TSharedPtr<IAnalyticsProvider> CreateAnalyticsProvider(const FAnalyticsProviderConfigurationDelegate& GetConfigValue) const override;


	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

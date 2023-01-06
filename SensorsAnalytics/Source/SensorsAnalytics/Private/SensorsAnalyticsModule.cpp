// Copyright Epic Games, Inc. All Rights Reserved.

#include "SensorsAnalyticsModule.h"
#include "Core.h"
#include "Json.h"
#include <string>
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"

#include "SensorsAnalytics.h"
#include "SensorsAnalyticsProvider.h"

IMPLEMENT_MODULE(FSensorsAnalyticsModule, SensorsAnalytics)

#define LOCTEXT_NAMESPACE "SensorsAnalyticsModule"

DEFINE_LOG_CATEGORY_STATIC(LogFSensorsAnalyticsModule, Display, All);

TSharedPtr<IAnalyticsProvider> FSensorAnalyticsProvider::Provider;

void FSensorsAnalyticsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

#if WITH_EDITOR
    // Register settings: `Settings -> Project Settings -> Plugins -> Sensors Analytics
    if (ISettingsModule* settingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        settingsModule->RegisterSettings("Project", "Plugins", "Sensors Analytics",
                                         LOCTEXT("RuntimeSettingsName", "Sensors Analytics"),
                                         LOCTEXT("RuntimeSettingsDescription", "Configure bundle(plugin)"),
                                         GetMutableDefault<USensorsAnalyticsSettings>());
    }
#endif
}

void FSensorsAnalyticsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
    if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
    {
        SettingsModule->UnregisterSettings("Project", "Plugins", "Sensors Analytics");
    }

}


TSharedPtr<IAnalyticsProvider> FSensorsAnalyticsModule::CreateAnalyticsProvider(const FAnalyticsProviderConfigurationDelegate& GetConfigValue) const
{
    return FSensorAnalyticsProvider::Create();
}



FSensorAnalyticsProvider::FSensorAnalyticsProvider() : bHasSessionStarted(false)
{
}

FSensorAnalyticsProvider::~FSensorAnalyticsProvider()
{
}

bool FSensorAnalyticsProvider::StartSession(const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::StartSession Success"));
    if (!bHasSessionStarted)
    {
        USensorsAnalytics::Start();
        bHasSessionStarted = true;
        UE_LOG(LogFSensorsAnalyticsModule, Display, TEXT("FSensorsAnalyticsModule::StartSession Success"));
    }
    return bHasSessionStarted;
}

void FSensorAnalyticsProvider::EndSession()
{
    if (bHasSessionStarted)
    {
        bHasSessionStarted = false;
        UE_LOG(LogFSensorsAnalyticsModule, Display, TEXT("FSensorsAnalyticsModule::EndSession Success"));
    }
}

void FSensorAnalyticsProvider::FlushEvents()
{
    USensorsAnalytics::Flush();
}


void FSensorAnalyticsProvider::SetUserID(const FString& InUserID)
{
    USensorsAnalytics::Login(InUserID);
}

FString FSensorAnalyticsProvider::GetUserID() const
{
    return "Unsupported";
}


FString FSensorAnalyticsProvider::GetSessionID() const
{
    return "Unsupported";
}

bool FSensorAnalyticsProvider::SetSessionID(const FString& InSessionID)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::SetSessionID: Ignoring"));
    return true;
}

void FSensorAnalyticsProvider::RecordEvent(const FString& EventName, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    if (!bHasSessionStarted)
    {
        UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordEvent called while a session is not started. Ignoring."));
        return;
    }

    if (EventName.Len() <= 0) 
    {
        UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordEvent called with invalid EventName. Ignoring"));
        return;
    }

    FString outStr;
    TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&outStr);
    /** Write JSON message */
    JsonWriter->WriteObjectStart();

    const int32 AttrCount = Attributes.Num();
    if (AttrCount > 0)
    {
        // Send an event for each attribute
        for (auto Attr : Attributes)
        {
            JsonWriter->WriteValue(*Attr.GetName(), *Attr.GetValue());
            // switch (Attr.AttrType) 
            // {
            //     case FAnalyticsEventAttribute::AttrTypeEnum::String:
            //         JsonWriter->WriteValue(*Attr.GetName(), *Attr.GetValue());
            //         break;
            //     case FAnalyticsEventAttribute::AttrTypeEnum::Boolean:
            //         JsonWriter->WriteValue(*Attr.GetName(), Attr.GetValue());
            //         break;
            //     case FAnalyticsEventAttribute::AttrTypeEnum::Number:
            //         JsonWriter->WriteValue(*Attr.GetName(), Attr.GetValue());
            //         break;
            //     default:
            //         JsonWriter->WriteValue(*Attr.GetName(), Attr.GetValue());
            //         break;
            // }
        }
    }
    JsonWriter->WriteObjectEnd();
    JsonWriter->Close();

    USensorsAnalytics::Track(EventName, outStr);
}

void FSensorAnalyticsProvider::RecordItemPurchase(const FString& ItemId, const FString& Currency, int PerItemCost, int ItemQuantity)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordItemPurchage: Ignoring"));
}

void FSensorAnalyticsProvider::RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const FString& RealCurrencyType, float RealMoneyCost, const FString& PaymentProvider)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordCurrencyPurchage: Ignoring"));
}

void FSensorAnalyticsProvider::RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordCurrencyGiven: Ignoring"));
}

void FSensorAnalyticsProvider::SetAge(int InAge)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::SetAge: Ignoring"));
}

void FSensorAnalyticsProvider::SetLocation(const FString& InLocation)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::SetLocation: Ignoring"));
}

void FSensorAnalyticsProvider::SetGender(const FString& InGender)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::SetGender: Ignoring"));
}

void FSensorAnalyticsProvider::SetBuildInfo(const FString& InBuildInfo)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::SetBuildInfo: Ignoring"));
}

void FSensorAnalyticsProvider::RecordError(const FString& Error, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordError: Ignoring"));
}

void FSensorAnalyticsProvider::RecordProgress(const FString& ProgressType, const FString& ProgressName, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordProgress: Ignoring"));
}

void FSensorAnalyticsProvider::RecordItemPurchase(const FString& ItemId, int ItemQuantity, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordItemPurchage: Ignoring"));
}


void FSensorAnalyticsProvider::RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordCurrencyPurchage: Ignoring"));
}


void FSensorAnalyticsProvider::RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& Attributes)
{
    UE_LOG(LogFSensorsAnalyticsModule, Warning, TEXT("FSensorsAnalyticsModule::RecordCurrencyGiven: Ignoring"));
}

#undef LOCTEXT_NAMESPACE

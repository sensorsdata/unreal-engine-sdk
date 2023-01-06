// Copyright 2022 SensorsData. All Rights Reserved.
#include "SensorsAnalyticsPC.h"

static SensorsAnalyticsPC *Manager;
static USAEventManager* EventManager;
static bool bEnableSDK;
static USensorsAnalyticsPCBase* BaseInstance;
static USensorsAnalyticsPCDisable* DisableInstance;

void SensorsAnalyticsPC::StartWithConfigOptions(const USensorsAnalyticsSettings * Settings)
{
    SetEnableLog(Settings->bEnableLog);

    if ((Settings->ServerUrl).IsEmpty())
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("ServerUrl is empty, start failed !"));
        return;
    }
    
    if (Manager == nullptr)
    {
        Manager = new SensorsAnalyticsPC();
       
        bEnableSDK = true;
        
        BaseInstance = NewObject<USensorsAnalyticsPCBase>();
        BaseInstance->StartWithConfigOptions(Settings);
        
        DisableInstance = NewObject<USensorsAnalyticsPCDisable>();
        DisableInstance->StartWithConfigOptions(Settings);
        
        EventManager = NewObject<USAEventManager>();
        EventManager->BindInstance(BaseInstance);
        EventManager->AddToRoot();
        
        BaseInstance->SetEventManager(EventManager);
                
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Initialize Success !"));
    }
    else
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Do not repeat initialization !"));
    }
}

void SensorsAnalyticsPC::EnableSDK()
{
    if (Manager == nullptr)
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("Please initialization first !"));
        return;
    }
    
    if (bEnableSDK)
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("SDK has been enabled !"));
        return;
    }
    
    FSALog::Warning(CUR_LOG_POSITION, TEXT("Enable SDK !"));
    
    bEnableSDK = true;
    EventManager->SetEnableManager(true);
}

void SensorsAnalyticsPC::DisableSDK()
{
    if (Manager == nullptr)
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("Please initialization first !"));
        return;
    }
    
    if (!(bEnableSDK))
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("SDK has been disabled !"));
        return;
    }
    
    FSALog::Warning(CUR_LOG_POSITION, TEXT("Disable SDK !"));
    
    bEnableSDK = false;
    EventManager->SetEnableManager(false);
}

void SensorsAnalyticsPC::RegisterSuperProperties(const FString& Properties)
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;
    
    Instance->RegisterSuperProperties(Properties);
}

void SensorsAnalyticsPC::UnregisterSuperProperty(const FString& Key)
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->UnregisterSuperProperty(Key);
}

void SensorsAnalyticsPC::ClearSuperProperties()
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->ClearSuperProperties();
}

void SensorsAnalyticsPC::Login(const FString& LoginId)
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->Login(LoginId);
}

void SensorsAnalyticsPC::Logout()
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->Logout();
}

void SensorsAnalyticsPC::Track(const FString& EventName, const FString& Properties)
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->Track(EventName, Properties);
}

void SensorsAnalyticsPC::TrackAppInstall(const FString& Properties, bool bDisableCallback)
{
    FSALog::Warning(CUR_LOG_POSITION, TEXT("Call SensorsAnalyticsPC::TrackAppInstall, the platform is not supported."));
}

void SensorsAnalyticsPC::SetOnce(const FString& Properties)
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->SetOnce(Properties);
}

void SensorsAnalyticsPC::Set(const FString& Properties)
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->Set(Properties);
}

void SensorsAnalyticsPC::Unset(const FString& Key)
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->Unset(Key);
}

void SensorsAnalyticsPC::Increment(const FString& Properties)
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->Increment(Properties);
}

void SensorsAnalyticsPC::Append(const FString& Key, const FString& Properties)
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->Append(Key, Properties);
}

void SensorsAnalyticsPC::DeleteUser()
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->DeleteUser();
}

void SensorsAnalyticsPC::Flush()
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->Flush();
}

void SensorsAnalyticsPC::DeleteAll()
{
    USensorsAnalyticsPCBase *Instance = Manager->GetInstance();
    if (Instance == nullptr) return;

    Instance->DeleteAll();
}

USensorsAnalyticsPCBase* SensorsAnalyticsPC::GetInstance()
{
    if (Manager == nullptr)
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("Please initialization first !"));
        return nullptr;
    }
    
    if (bEnableSDK)
    {
        return BaseInstance;
    }
    else
    {
        return DisableInstance;
    }
}

void SensorsAnalyticsPC::SetEnableLog(bool Enable)
{
    FSALog::SetEnableLog(Enable);
}

// Copyright 2022 SensorsData. All Rights Reserved.

#include "SAEventManager.h"
#include "SATaskHandle.h"
#include "SASaveEvent.h"
#include "SensorsAnalyticsPCBase.h"

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#include "Editor/UnrealEdEngine.h"
#else
#include "Engine/GameEngine.h"
#endif

USAEventManager::USAEventManager()
{

}

USAEventManager::~USAEventManager()
{
	// FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT(" ~USAEventManager ")));
}

void USAEventManager::EnqueueUserEvent(const FString& EventType, const FString& Properties)
{
    FSALog::Warning(CUR_LOG_POSITION, TEXT("Track user event with type: ") + EventType + TEXT(", properties: ") + (Properties.IsEmpty() ? TEXT("{}") : Properties));
    
    TSharedPtr<FJsonObject> m_PropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> m_LibJsonObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> m_IdentitiesJsonObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
    
    TSharedPtr<FJsonObject> LibPropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> LibPropertiesReader = TJsonReaderFactory<>::Create(m_Instance->sa_GetLibProperties());
    FJsonSerializer::Deserialize(LibPropertiesReader, LibPropertiesJsonObject);
    for (auto& Elem : LibPropertiesJsonObject->Values)
    {
        m_LibJsonObject->SetField(Elem.Key, Elem.Value);
    }
    
    TSharedPtr<FJsonObject> IdentitiesPropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> IdentitiesPropertiesReader = TJsonReaderFactory<>::Create(m_Instance->sa_GetIdentitiesProperties());
    FJsonSerializer::Deserialize(IdentitiesPropertiesReader, IdentitiesPropertiesJsonObject);
    for (auto& Elem : IdentitiesPropertiesJsonObject->Values)
    {
        m_IdentitiesJsonObject->SetField(Elem.Key, Elem.Value);
    }
    
    FString LoginID = m_Instance->sa_GetLoginID();
    if (!LoginID.IsEmpty())
    {
        m_IdentitiesJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_IDENTITY_LOGIN_ID, LoginID);
    }
    
    TSharedPtr<FJsonObject> PropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> PropertiesReader = TJsonReaderFactory<>::Create(Properties);
    FJsonSerializer::Deserialize(PropertiesReader, PropertiesJsonObject);
    
    for (auto& Elem : PropertiesJsonObject->Values)
    {
        if (FSAUtils::IsInvalidName(Elem.Key))
        {
            FSALog::Error(CUR_LOG_POSITION, TEXT("Property key is Error !"));
            continue;
        }
        
//        if (FSAUtils::IsInvalidValue(Elem.Value))
//        {
//            FSALog::Error(CUR_LOG_POSITION, TEXT("Property value is Error !"));
//        }
        
        m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
    }

    m_DataJsonObject->SetNumberField(FSAConstants::SA_EVENT_TRACK_ID, FSAUtils::GetTrackID());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_ANONYMOUS_ID, m_Instance->sa_GetAnonymousID());
	m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_DISTINCT_ID, m_Instance->sa_GetDistinctID());

    m_DataJsonObject->SetObjectField(FSAConstants::SA_EVENT_IDENTITIES, m_IdentitiesJsonObject);
    
    m_DataJsonObject->SetObjectField(FSAConstants::SA_EVENT_LIB, m_LibJsonObject);
    
    if (!LoginID.IsEmpty())
    {
        m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_LOGIN_ID, LoginID);
    }
    
	m_DataJsonObject->SetObjectField(FSAConstants::SA_EVENT_PROPERTIES, m_PropertiesJsonObject);
    
    m_DataJsonObject->SetNumberField(FSAConstants::SA_EVENT_TIME, FSAUtils::GetCurrentTimeStamp());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_TYPE, EventType);

	FString DataStr;
	TSharedRef<TJsonWriter<>> DataWriter = TJsonWriterFactory<>::Create(&DataStr);
	FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), DataWriter);
    
    FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("\n【track event】\n %s"), *DataStr));

	m_TaskHandle->AddTask(DataStr);
}

void USAEventManager::EnqueueTrackEvent(const FString& EventName, const FString& Properties, SAEventType type)
{
    FSALog::Warning(CUR_LOG_POSITION, TEXT("EventName: ") + EventName + TEXT(", and properties: ") + (Properties.IsEmpty() ? TEXT("{}") : Properties));
    
    if (FSAUtils::IsInvalidName(EventName))
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("Event name is Error !"));
    }
    
    TSharedPtr<FJsonObject> m_PropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> m_LibJsonObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> m_IdentitiesJsonObject = MakeShareable(new FJsonObject);
    TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
    
    TSharedPtr<FJsonObject> PresetPropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> PresetPropertiesReader = TJsonReaderFactory<>::Create(m_Instance->sa_GetPresetProperties());
    FJsonSerializer::Deserialize(PresetPropertiesReader, PresetPropertiesJsonObject);
    for (auto& Elem : PresetPropertiesJsonObject->Values)
    {
        m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
    }
    
    if (type == SAEventType::TRACK)
    {
        FString CurrentTime = FSAUtils::GetCurrentFormatTime();
        FString FirstDayTime = m_Instance->sa_GetFirstDayTime();
        bool IsFirstDay = false;
        if (!CurrentTime.IsEmpty() && !FirstDayTime.IsEmpty())
        {
            IsFirstDay = CurrentTime.Equals(FirstDayTime);
        }
        
        m_PropertiesJsonObject->SetBoolField(FSAConstants::SA_EVENT_PRESET_PROPERTY_IS_FIRST_DAY, IsFirstDay);
    }
    
    TSharedPtr<FJsonObject> LibPropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> LibPropertiesReader = TJsonReaderFactory<>::Create(m_Instance->sa_GetLibProperties());
    FJsonSerializer::Deserialize(LibPropertiesReader, LibPropertiesJsonObject);
    for (auto& Elem : LibPropertiesJsonObject->Values)
    {
        m_LibJsonObject->SetField(Elem.Key, Elem.Value);
    }
    
    TSharedPtr<FJsonObject> IdentitiesPropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> IdentitiesPropertiesReader = TJsonReaderFactory<>::Create(m_Instance->sa_GetIdentitiesProperties());
    FJsonSerializer::Deserialize(IdentitiesPropertiesReader, IdentitiesPropertiesJsonObject);
    for (auto& Elem : IdentitiesPropertiesJsonObject->Values)
    {
        m_IdentitiesJsonObject->SetField(Elem.Key, Elem.Value);
    }
    
    FString LoginID = m_Instance->sa_GetLoginID();
    if (!LoginID.IsEmpty())
    {
        m_IdentitiesJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_IDENTITY_LOGIN_ID, LoginID);
    }
    
    TSharedPtr<FJsonObject> SuperPropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> SuperPropertiesReader = TJsonReaderFactory<>::Create(m_Instance->sa_GetSuperProperties());
    FJsonSerializer::Deserialize(SuperPropertiesReader, SuperPropertiesJsonObject);
    
    TSharedPtr<FJsonObject> PropertiesJsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> PropertiesReader = TJsonReaderFactory<>::Create(Properties);
    FJsonSerializer::Deserialize(PropertiesReader, PropertiesJsonObject);
    
    for (auto& Elem : SuperPropertiesJsonObject->Values)
    {
        m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
    }
    
    for (auto& Elem : PropertiesJsonObject->Values)
    {
        if (FSAUtils::IsInvalidName(Elem.Key))
        {
            FSALog::Error(CUR_LOG_POSITION, TEXT("Property key is Error !"));
            continue;
        }
        
//        if (FSAUtils::IsInvalidValue(Elem.Value))
//        {
//            FSALog::Error(CUR_LOG_POSITION, TEXT("Property value is Error !"));
//        }
        
        m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
    }
    
    m_DataJsonObject->SetNumberField(FSAConstants::SA_EVENT_TRACK_ID, FSAUtils::GetTrackID());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_ANONYMOUS_ID, m_Instance->sa_GetAnonymousID());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_DISTINCT_ID, m_Instance->sa_GetDistinctID());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_NAME, EventName);
    
    m_DataJsonObject->SetObjectField(FSAConstants::SA_EVENT_IDENTITIES, m_IdentitiesJsonObject);

    m_DataJsonObject->SetObjectField(FSAConstants::SA_EVENT_LIB, m_LibJsonObject);
    
    if (!LoginID.IsEmpty())
    {
        m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_LOGIN_ID, LoginID);
    }
    
    if (type == SAEventType::TRACK_SIGNUP)
    {
        m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_ORIGINAL_ID, m_Instance->sa_GetAnonymousID());
    }
    
    m_DataJsonObject->SetObjectField(FSAConstants::SA_EVENT_PROPERTIES, m_PropertiesJsonObject);
    
    m_DataJsonObject->SetNumberField(FSAConstants::SA_EVENT_TIME, FSAUtils::GetCurrentTimeStamp());
    
    if (type == SAEventType::TRACK_SIGNUP)
    {
        m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_TYPE, FSAConstants::SA_EVENT_TYPE_TRACK_SIGNUP);
    }
    else
    {
        m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_TYPE, FSAConstants::SA_EVENT_TYPE_TRACK);
    }
    
    FString DataStr;
    TSharedRef<TJsonWriter<>> DataWriter = TJsonWriterFactory<>::Create(&DataStr);
    FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), DataWriter);
    
    FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("\n【track event】\n %s"), *DataStr));
    
    m_TaskHandle->AddTask(DataStr);
}

void USAEventManager::BindInstance(USensorsAnalyticsPCBase *Instance)
{
	m_Instance = Instance;
	//start timer
	//event num & time
	m_GameInstance = GetGameInstance();
	m_GameInstance->AddToRoot();
	m_GameInstance->GetTimerManager().SetTimer(WorkHandle, this, &USAEventManager::Flush, m_Instance->sa_GetFlushInterval(), true);

	m_TaskHandle = new FSATaskHandle(Instance);
    m_RunnableThread = FRunnableThread::Create(m_TaskHandle, TEXT("SATaskHandle"), 128 * 1024, TPri_AboveNormal, FPlatformAffinity::GetPoolThreadMask());
}

void USAEventManager::SetEnableManager(bool Enable)
{
    m_TaskHandle->SetEnableTask(Enable);
}

void USAEventManager::Flush()
{
    m_TaskHandle->ResetWorkingFlag();
    m_TaskHandle->AddTask(TEXT(""));
}

UGameInstance* USAEventManager::GetGameInstance()
{
	UGameInstance* GameInstance = nullptr;
#if WITH_EDITOR
	UUnrealEdEngine* engine = Cast<UUnrealEdEngine>(GEngine);
	if (engine && engine->PlayWorld) GameInstance = engine->PlayWorld->GetGameInstance();
#else
	UGameEngine* engine = Cast<UGameEngine>(GEngine);
	if (engine) GameInstance = engine->GameInstance;
#endif
	return GameInstance;
}

void USAEventManager::DeleteAllEvents()
{
    m_TaskHandle->DeleteAllFromLocal();
}


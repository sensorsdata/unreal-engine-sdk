// Copyright 2022 SensorsData. All Rights Reserved.
#include "SensorsAnalyticsPCBase.h"

USensorsAnalyticsPCBase::USensorsAnalyticsPCBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

USensorsAnalyticsPCBase::~USensorsAnalyticsPCBase()
{
    
}

void USensorsAnalyticsPCBase::StartWithConfigOptions(const USensorsAnalyticsSettings * Settings)
{
    this->Init(Settings->ServerUrl, Settings->FlushInterval, Settings->FlushBulkSize);
    this->AddToRoot();

    // 本地读取配置
    this->SlotName = FString(TEXT("SensorsAnalyticsSlotName"));
    this->m_SaveConfig = this->ReadValue();
    
    this->m_DistinctID = this->m_SaveConfig->m_DistinctID;
    this->m_AnonymousID = this->m_SaveConfig->m_AnonymousID;
    this->m_DeviceID = this->m_SaveConfig->m_DeviceID;
    this->m_SerialID = this->m_SaveConfig->m_SerialID;
    this->m_UUID = this->m_SaveConfig->m_UUID;
    this->m_FirstDayTime = this->m_SaveConfig->m_FirstDayTime;

    this->m_bIsAppInstall = this->m_SaveConfig->m_bIsAppInstall;
    this->m_LoginId = this->m_SaveConfig->m_LoginId;
    this->m_SuperProperties = this->m_SaveConfig->m_SuperProperties;
    
    this->m_SaveConfig->AddToRoot();
    
    // 初始化属性
    this->InitPresetProperties();
    this->InitLibProperties();
    this->InitIdentitiesProperties();
}

void USensorsAnalyticsPCBase::Init(const FString& ServerUrl, int FlushInterval, int FlushBulkSize)
{
    this->m_ServerUrl = ServerUrl;
    // FlushInterval is in milliseconds
    this->m_FlushInterval = (FlushInterval < 5000) ? 5 : (FlushInterval / 1000);
    this->m_FlushBulkSize = (FlushBulkSize < 50) ? 50 : FlushBulkSize;
}

void USensorsAnalyticsPCBase::RegisterSuperProperties(const FString& Properties)
{
    FString FinalProperties = FSAUtils::MergeProperties(Properties, this->m_SuperProperties);
    this->m_SuperProperties = FinalProperties;
    this->m_SaveConfig->SetSuperProperties(this->m_SuperProperties);
    this->SaveValue(this->m_SaveConfig);
}

void USensorsAnalyticsPCBase::UnregisterSuperProperty(const FString& Key)
{
    FString FinalProperties = FSAUtils::RemovePropertyWithKey(this->m_SuperProperties, Key);
    this->m_SuperProperties = FinalProperties;
    this->m_SaveConfig->SetSuperProperties(this->m_SuperProperties);
    this->SaveValue(this->m_SaveConfig);
}

void USensorsAnalyticsPCBase::ClearSuperProperties()
{
    this->m_SuperProperties = "";
    this->m_SaveConfig->SetSuperProperties(this->m_SuperProperties);
    this->SaveValue(this->m_SaveConfig);
}

void USensorsAnalyticsPCBase::Login(const FString& LoginId)
{
    if (FSAUtils::IsInvalidValue(LoginId))
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("LoginId is Error !"));
        return;
    }
    
    // incorrect LoginId
    if (this->m_AnonymousID == LoginId)
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("LoginID: ") + LoginId + TEXT(" is same with anonymous ID: " + this->m_AnonymousID));
        return;
    }

    if (this->m_LoginId == LoginId)
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("LoginID: ") + LoginId + TEXT(" is not change !"));
        return;
    }
    
    this->m_LoginId = LoginId;
    this->m_SaveConfig->SetLoginId(this->m_LoginId);
    
    this->m_DistinctID = LoginId;
    this->m_SaveConfig->SetDistinctID(this->m_DistinctID);

    this->SaveValue(this->m_SaveConfig);
    
    FSALog::Warning(CUR_LOG_POSITION, TEXT("Login with id: ") + LoginId);
    
    this->m_EventManager->EnqueueTrackEvent(FSAConstants::SA_EVENT_NAME_SIGN_UP, TEXT(""), SAEventType::TRACK_SIGNUP);
    
    this->Flush();
}

void USensorsAnalyticsPCBase::Logout()
{
    this->m_LoginId = "";
    this->m_SaveConfig->SetLoginId(this->m_LoginId);
    
    this->m_DistinctID = this->sa_GetAnonymousID();
    this->m_SaveConfig->SetDistinctID(this->m_DistinctID);
    
    this->SaveValue(this->m_SaveConfig);
    
    FSALog::Warning(CUR_LOG_POSITION, TEXT("Logout"));
}

void USensorsAnalyticsPCBase::Track(const FString& EventName, const FString& Properties)
{
    this->m_EventManager->EnqueueTrackEvent(EventName, Properties, SAEventType::TRACK);
}

void USensorsAnalyticsPCBase::TrackAppInstall(const FString& Properties, bool bDisableCallback)
{
    if (this->m_bIsAppInstall)
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("$AppInstall has been tracked !"));
        return;
    }
    
    this->m_bIsAppInstall = true;
    this->m_SaveConfig->SetAppInstall(true);
    this->SaveValue(this->m_SaveConfig);
    
    if (bDisableCallback)
    {
        TSharedPtr<FJsonObject> m_PropertiesJsonObject = MakeShareable(new FJsonObject);
        
        m_PropertiesJsonObject->SetBoolField(FSAConstants::SA_EVENT_PRESET_PROPERTY_INSTALL_DISABLE_CALLBACK, true);

        TSharedPtr<FJsonObject> PropertiesJsonObject = MakeShareable(new FJsonObject);
        TSharedRef<TJsonReader<>> PropertiesReader = TJsonReaderFactory<>::Create(Properties);
        FJsonSerializer::Deserialize(PropertiesReader, PropertiesJsonObject);
        for (auto& Elem : PropertiesJsonObject->Values)
        {
            m_PropertiesJsonObject->SetField(Elem.Key, Elem.Value);
        }
        
        FString JsonStr;
        TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
        FJsonSerializer::Serialize(m_PropertiesJsonObject.ToSharedRef(), Writer);
        
        this->m_EventManager->EnqueueTrackEvent(FSAConstants::SA_EVENT_NAME_APP_INSTALL, JsonStr, SAEventType::TRACK);
    }
    else
    {
        this->m_EventManager->EnqueueTrackEvent(FSAConstants::SA_EVENT_NAME_APP_INSTALL, Properties, SAEventType::TRACK);
    }
}

void USensorsAnalyticsPCBase::SetOnce(const FString& Properties)
{
    this->m_EventManager->EnqueueUserEvent(FString(FSAConstants::SA_EVENT_TYPE_PROFILE_SET_ONCE), Properties);
}

void USensorsAnalyticsPCBase::Set(const FString& Properties)
{
    
    this->m_EventManager->EnqueueUserEvent(FString(FSAConstants::SA_EVENT_TYPE_PROFILE_SET), Properties);
}

void USensorsAnalyticsPCBase::Unset(const FString& Key)
{
    TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
    m_DataJsonObject->SetStringField(Key, TEXT(""));
    FString DataStr;
    TSharedRef<TJsonWriter<>> DataWriter = TJsonWriterFactory<>::Create(&DataStr);
    FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), DataWriter);

    this->m_EventManager->EnqueueUserEvent(FString(FSAConstants::SA_EVENT_TYPE_PROFILE_UNSET), DataStr);
}

void USensorsAnalyticsPCBase::Increment(const FString& Properties)
{
    this->m_EventManager->EnqueueUserEvent(FString(FSAConstants::SA_EVENT_TYPE_PROFILE_INCREMENT), Properties);
}

void USensorsAnalyticsPCBase::Append(const FString& Key, const FString& Properties)
{
    if (Key.IsEmpty())
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("Append key is empty !"));
        return;
    }
    if (Properties.IsEmpty())
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("Append properties is empty !"));
        return;
    }
    
    FString ReplaceProperties = Properties;
    ReplaceProperties.ReplaceInline(TEXT("["), TEXT(""));
    ReplaceProperties.ReplaceInline(TEXT("]"), TEXT(""));
    ReplaceProperties.ReplaceInline(TEXT("\""), TEXT(""));
    if (ReplaceProperties.IsEmpty())
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("Append properties is invalid !"));
        return;
    }
    
    TArray< TSharedPtr<FJsonValue> > PropertiesArray;
    TArray<FString> TempArray;
    ReplaceProperties.ParseIntoArray(TempArray, TEXT(","), true);
    if (TempArray.Num() > 0)
    {
        for (int i = 0; i < TempArray.Num(); i++)
        {
            FString TempString = TempArray[i];
            TempString.TrimStartAndEndInline();
            PropertiesArray.Add(MakeShareable(new FJsonValueString(TempString)));
        }
    }
    
    if (PropertiesArray.Num() > 0)
    {
        TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
        m_DataJsonObject->SetArrayField(Key, PropertiesArray);
        FString DataStr;
        TSharedRef<TJsonWriter<>> DataWriter = TJsonWriterFactory<>::Create(&DataStr);
        FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), DataWriter);
        
        this->m_EventManager->EnqueueUserEvent(FString(FSAConstants::SA_EVENT_TYPE_PROFILE_APPEND), DataStr);
    }
    else
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("Append properties is not array !"));
    }
}

void USensorsAnalyticsPCBase::DeleteUser()
{
    this->m_EventManager->EnqueueUserEvent(FString(FSAConstants::SA_EVENT_TYPE_PROFILE_DELETE), TEXT(""));
}

void USensorsAnalyticsPCBase::Flush()
{
    this->m_EventManager->Flush();
}

void USensorsAnalyticsPCBase::DeleteAll()
{
    this->m_EventManager->DeleteAllEvents();
}

bool USensorsAnalyticsPCBase::IsEnableSDK()
{
    return true;
}

void USensorsAnalyticsPCBase::SetEventManager(USAEventManager * InputEventManager)
{
    this->m_EventManager = InputEventManager;
}

FString USensorsAnalyticsPCBase::sa_GetDistinctID()
{
    return this->m_DistinctID;
}

FString USensorsAnalyticsPCBase::sa_GetAnonymousID()
{
    return this->m_AnonymousID;
}

FString USensorsAnalyticsPCBase::sa_GetDeviceID()
{
    return this->m_DeviceID;
}

FString USensorsAnalyticsPCBase::sa_GetSerialID()
{
    return this->m_SerialID;
}

FString USensorsAnalyticsPCBase::sa_GetLoginID()
{
    return this->m_LoginId;
}

FString USensorsAnalyticsPCBase::sa_GetSuperProperties()
{
    return this->m_SuperProperties;
}

FString USensorsAnalyticsPCBase::sa_GetPresetProperties()
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> SuperPropertiesReader = TJsonReaderFactory<>::Create(this->m_PresetProperties);
    FJsonSerializer::Deserialize(SuperPropertiesReader, JsonObject);

    FString JsonStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    return JsonStr;
}

FString USensorsAnalyticsPCBase::sa_GetLibProperties()
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> LibPropertiesReader = TJsonReaderFactory<>::Create(this->m_LibProperties);
    FJsonSerializer::Deserialize(LibPropertiesReader, JsonObject);

    FString JsonStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    return JsonStr;
}

FString USensorsAnalyticsPCBase::sa_GetIdentitiesProperties()
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> IdentitiesPropertiesReader = TJsonReaderFactory<>::Create(this->m_IdentitiesProperties);
    FJsonSerializer::Deserialize(IdentitiesPropertiesReader, JsonObject);

    FString JsonStr;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonStr);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    return JsonStr;
}

FString USensorsAnalyticsPCBase::sa_GetServerUrl()
{
    return this->m_ServerUrl;
}

float USensorsAnalyticsPCBase::sa_GetFlushInterval()
{
    return this->m_FlushInterval;
}

int USensorsAnalyticsPCBase::sa_GetFlushBulkSize()
{
    return this->m_FlushBulkSize;
}

FString USensorsAnalyticsPCBase::sa_GetUUID()
{
    return this->m_UUID;
}

FString USensorsAnalyticsPCBase::sa_GetFirstDayTime()
{
    return this->m_FirstDayTime;
}

void USensorsAnalyticsPCBase::InitIdentitiesProperties()
{
    TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
    
    FString SerialID = sa_GetSerialID();
    FString UUID = sa_GetUUID();

    if (SerialID.IsEmpty())
    {
#if PLATFORM_MAC
        m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_IDENTITY_MAC_UUID, UUID);
#elif PLATFORM_WINDOWS
        m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_IDENTITY_WINDOWS_UUID, UUID);
#endif
    }
    else
    {
#if PLATFORM_MAC
        m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_IDENTITY_MAC_SERIAL_ID, SerialID);
#elif PLATFORM_WINDOWS
        m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_IDENTITY_WINDOWS_SERIAL_ID, SerialID);
#endif
    }

    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&m_IdentitiesProperties);
    FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), Writer);
}

void USensorsAnalyticsPCBase::InitLibProperties()
{
    TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_APP_VERSION, FSAUtils::GetProjectVersion());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_LIB, FSAConstants::SA_PC_SDK_NAME);
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_LIB_VERSION, FSAConstants::SA_PC_SDK_VERSION);
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_LIB_METHOD, TEXT("code"));
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&m_LibProperties);
    FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), Writer);
}

void USensorsAnalyticsPCBase::InitPresetProperties()
{
    TSharedPtr<FJsonObject> m_DataJsonObject = MakeShareable(new FJsonObject);
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_APP_VERSION, FSAUtils::GetProjectVersion());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_LIB, FSAConstants::SA_PC_SDK_NAME);
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_LIB_VERSION, FSAConstants::SA_PC_SDK_VERSION);
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_LIB_METHOD, TEXT("code"));
    m_DataJsonObject->SetNumberField(FSAConstants::SA_EVENT_PRESET_PROPERTY_SCREEN_WIDTH, FSAUtils::GetScreenWidth());
    m_DataJsonObject->SetNumberField(FSAConstants::SA_EVENT_PRESET_PROPERTY_SCREEN_HEIGHT, FSAUtils::GetScreenHeight());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_OS, FSAUtils::GetOS());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_OS_VERSION, FSAUtils::GetOSVersion());
    m_DataJsonObject->SetStringField(FSAConstants::SA_EVENT_PRESET_PROPERTY_DEVICE_ID, sa_GetDeviceID());
    m_DataJsonObject->SetArrayField(FSAConstants::SA_EVENT_PRESET_PROPERTY_DATA_INGESTION_SOURCE, FSAUtils::GetDataIngestionSource());
    m_DataJsonObject->SetNumberField(FSAConstants::SA_EVENT_PRESET_PROPERTY_TIMEZONE_OFFSET, FSAUtils::GetTimezoneOffset());
    
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&m_PresetProperties);
    FJsonSerializer::Serialize(m_DataJsonObject.ToSharedRef(), Writer);
}

USASaveConfig* USensorsAnalyticsPCBase::ReadValue()
{
    FSALog::Warning(CUR_LOG_POSITION, TEXT("ReadValue with slot name: ") + this->SlotName);

    USASaveConfig* SaveConfig = Cast<USASaveConfig>(UGameplayStatics::LoadGameFromSlot(this->SlotName, FSAConstants::SA_SLOT_USER_INDEX_CONFIG));
    if( !SaveConfig )
    {
        SaveConfig = Cast<USASaveConfig>(UGameplayStatics::CreateSaveGameObject(USASaveConfig::StaticClass()));
        this->m_DistinctID = FSAUtils::GetMachineLoginId();
        this->m_AnonymousID = FSAUtils::GetAnonymousID();
        this->m_DeviceID = FSAUtils::GetDeviceID();
        this->m_SerialID = FSAUtils::GetSerialID();
        this->m_UUID = FSAUtils::GetUUID();
        this->m_FirstDayTime = FSAUtils::GetCurrentFormatTime();

        SaveConfig->SetDistinctID(this->m_DistinctID);
        SaveConfig->SetAnonymousID(this->m_AnonymousID);
        SaveConfig->SetDeviceID(this->m_DeviceID);
        SaveConfig->SetSerialID(this->m_SerialID);
        SaveConfig->SetUUID(this->m_UUID);
        SaveConfig->SetFirstDayTime(this->m_FirstDayTime);

        UGameplayStatics::SaveGameToSlot(SaveConfig, this->SlotName, FSAConstants::SA_SLOT_USER_INDEX_CONFIG);
        FSALog::Warning(CUR_LOG_POSITION, TEXT("ReadValue CreateSaveGameObject Success !"));
    }
    FSALog::Warning(CUR_LOG_POSITION, TEXT("ReadValue Success !"));
    return SaveConfig;
}

void USensorsAnalyticsPCBase::SaveValue(USASaveConfig *SaveConfig)
{
    FSALog::Warning(CUR_LOG_POSITION, TEXT("SaveValue with slot name: ") + this->SlotName);

    if( !SaveConfig )
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("SaveValue CreateSaveGameObject Success !"));
        SaveConfig = Cast<USASaveConfig>(UGameplayStatics::CreateSaveGameObject(USASaveConfig::StaticClass()));
    }
    UGameplayStatics::SaveGameToSlot(SaveConfig, this->SlotName, FSAConstants::SA_SLOT_USER_INDEX_CONFIG);
}

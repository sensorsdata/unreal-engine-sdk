// Copyright 2022 SensorsData. All Rights Reserved.
#include "SAUtils.h"
#include "PlatformHttp.h"

FString FSAUtils::Pattern = TEXT("^((?!^distinct_id$|^original_id$|^time$|^properties$|^id$|^first_id$|^second_id$|^users$|^events$|^event$|^user_id$|^date$|^datetime$|^user_tag.*|^user_group.*)[a-zA-Z_$][a-zA-Z\\d_$]*)$");

FString FSAUtils::EncodeData(const FString& UnprocessedStr)
{
    // Compatible with Chinese
    FTCHARToUTF8 ToUtf8Converter(UnprocessedStr.GetCharArray().GetData());

    // Gzip Data
    auto UnprocessedDataLen = ToUtf8Converter.Length();
    auto UnprocessedData = ToUtf8Converter.Get();
    int32 CompressBufferLen = FCompression::CompressMemoryBound(NAME_Gzip, UnprocessedDataLen);
    void* CompressBuffer = FMemory::Malloc(CompressBufferLen);
    bool Result = FCompression::CompressMemory(NAME_Gzip, CompressBuffer, CompressBufferLen, UnprocessedData,
        UnprocessedDataLen, ECompressionFlags::COMPRESS_BiasSpeed);

    FString ResultStr;
    if (Result)
    {
        // Base64 Encode
        FString Base64EncodeStr = FBase64::Encode((uint8*)CompressBuffer, CompressBufferLen);
        if (!Base64EncodeStr.IsEmpty())
        {
            // Url Encode
            FString UrlEncodeStr = FPlatformHttp::UrlEncode(Base64EncodeStr);
            if (!UrlEncodeStr.IsEmpty())
            {
                ResultStr = TEXT("data_list=") + UrlEncodeStr + TEXT("&gzip=1");
            } 
            else
            {
                FSALog::Error(CUR_LOG_POSITION, TEXT("Url Encode Data Error !"));
            }
        }
        else
        {
            FSALog::Error(CUR_LOG_POSITION, TEXT("Base64 Encode Data Error !"));
        }
    }
    else
    {
        FSALog::Error(CUR_LOG_POSITION, TEXT("Gzip Data Error !"));
    }
    FMemory::Free(CompressBuffer);
	return  ResultStr;
}

int64 FSAUtils::GetCurrentTimeStamp()
{
    FDateTime TDateTime = FDateTime::UtcNow();
    int64 SecondTimestamp = TDateTime.ToUnixTimestamp();
    int32 MillisecondPart = TDateTime.GetMillisecond();
    int64 Timestamp = SecondTimestamp * 1000 + MillisecondPart;
    return Timestamp;
}

FString FSAUtils::GetGuid()
{
    FGuid Guid;
    FGenericPlatformMisc::CreateGuid(Guid);
    return Guid.ToString();
}

bool FSAUtils::IsInvalidName(const FString& Name)
{
	if (&Name == nullptr)
	{
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Name is nullptr !"));
		return true;
	}
    
    if (Name.IsEmpty())
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Name is empty !"));
        return true;
    }
    
    if (Name.Len() > FSAConstants::SA_EVENT_NAME_MAX_LENGTH)
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Name is longer than max length !"));
        return true;
    }
    
	FRegexPattern MatherPatter(Pattern);
	FRegexMatcher Matcher(MatherPatter, Name);
	while (!Matcher.FindNext())
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Name [") + Name + TEXT("] is not valid !"));
        return true;
    }

	return false;
}

bool FSAUtils::IsInvalidValue(const FString& Value)
{
    if (&Value == nullptr)
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Value is nullptr !"));
        return true;
    }
    
    if (Value.IsEmpty())
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Value is empty !"));
        return true;
    }
    
    if (Value.Len() > FSAConstants::SA_PROPERTY_VALUE_MAX_LENGTH)
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("Value is longer than max length !"));
        return true;
    }
    
    return false;
}

FString FSAUtils::GetOS()
{
#if PLATFORM_MAC
    return TEXT("macOS");
#else
    return FPlatformProperties::IniPlatformName();
#endif
}

FString FSAUtils::GetOSVersion()
{
	return FPlatformMisc::GetOSVersion();
}


FString FSAUtils::GetMachineLoginId()
{
	return FPlatformMisc::GetLoginId();
}

FString FSAUtils::GetAnonymousID()
{
    return FPlatformMisc::GetLoginId();
}

FString FSAUtils::GetUUID()
{
    FGuid Guid;
    FGenericPlatformMisc::CreateGuid(Guid);
    return Guid.ToString();
}

FString FSAUtils::GetDeviceID()
{
//    return FGenericPlatformMisc::GetDeviceId();
    return FPlatformMisc::GetLoginId();
}

FString FSAUtils::GetSerialID()
{
    return FPlatformMisc::GetLoginId();
}

FString FSAUtils::GetCurrentFormatTime()
{
    // for example: 2023-01-01
    FDateTime TDateTime = FDateTime::UtcNow();
    return TDateTime.ToString(TEXT("%Y-%m-%d"));
}

FString FSAUtils::GetEngineVersion()
{
	return FEngineAnalytics::GetProvider().GetAppVersion();
}

FString FSAUtils::GetProjectVersion()
{
	const UGeneralProjectSettings& ProjectSettings = *GetDefault<UGeneralProjectSettings>();
	return ProjectSettings.ProjectVersion;
}

uint32 FSAUtils::GetScreenWidth()
{
	return GSystemResolution.ResX;
}

uint32 FSAUtils::GetScreenHeight()
{
	return GSystemResolution.ResY;
}

TArray< TSharedPtr<FJsonValue> > FSAUtils::GetDataIngestionSource()
{
    TArray< TSharedPtr<FJsonValue> > SourceArray;
    SourceArray.Add(MakeShareable(new FJsonValueString(FSAConstants::SA_PC_SDK_NAME)));
    return SourceArray;
}

FString FSAUtils::MergeProperties(const FString& FirstProperty, const FString& SecondProperty)
{
    FSALog::Warning(CUR_LOG_POSITION, TEXT("Merge properties with first property: ") + FirstProperty + TEXT(" and second property: ") + SecondProperty);

    FString RetStr;

    TSharedPtr<FJsonObject> FirstDataObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> FirstReader = TJsonReaderFactory<>::Create(FirstProperty);
    FJsonSerializer::Deserialize(FirstReader, FirstDataObject);

    TSharedPtr<FJsonObject> SecondDataObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> SecondReader = TJsonReaderFactory<>::Create(SecondProperty);
    FJsonSerializer::Deserialize(SecondReader, SecondDataObject);

    for (auto& Elem : FirstDataObject->Values)
    {
        SecondDataObject->SetField(Elem.Key, Elem.Value);
    }

    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RetStr);
    FJsonSerializer::Serialize(SecondDataObject.ToSharedRef(), Writer);

    FSALog::Warning(CUR_LOG_POSITION, TEXT("Merge properties result is: ") + RetStr);
    
    return RetStr;
}

FString FSAUtils::RemovePropertyWithKey(const FString& Properties, const FString& Key)
{
    FSALog::Warning(CUR_LOG_POSITION, TEXT("Remove property with properties: ") + Properties + TEXT(" and key: ") + Key);

    FString RetStr;

    TSharedPtr<FJsonObject> DataObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Properties);
    FJsonSerializer::Deserialize(Reader, DataObject);
    
    DataObject->RemoveField(Key);
    
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RetStr);
    FJsonSerializer::Serialize(DataObject.ToSharedRef(), Writer);
    
    FSALog::Warning(CUR_LOG_POSITION, TEXT("Remove property result is: ") + RetStr);

    return RetStr;
}

int64 FSAUtils::GetTrackID()
{
    std::string first = std::to_string(rand() % 99 + 100);
    std::string first_sub = first.substr(first.length() - 2, 2);

    std::string second = std::to_string(rand() % 999 + 1000);
    std::string second_sub = second.substr(second.length() - 3, 3);

    std::string timestamp = std::to_string(GetCurrentTimeStamp());
    std::string timestamp_sub = timestamp.substr(timestamp.length() - 4, 4);

    return std::stoi(first_sub + second_sub + timestamp_sub);
}

FString FSAUtils::AddPluginVersion(const FString& properties, const FString& key, const FString& value)
{
    FString RetStr;

    TSharedPtr<FJsonObject> PropertiesDataObject = MakeShareable(new FJsonObject);
    TSharedRef<TJsonReader<>> PropertiesReader = TJsonReaderFactory<>::Create(properties);
    FJsonSerializer::Deserialize(PropertiesReader, PropertiesDataObject);
    
    TArray< TSharedPtr<FJsonValue> > VersionArray;
    VersionArray.Add(MakeShareable(new FJsonValueString(value)));
    
    PropertiesDataObject->SetArrayField(key, VersionArray);

    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RetStr);
    FJsonSerializer::Serialize(PropertiesDataObject.ToSharedRef(), Writer);

    return RetStr;
}

int32 FSAUtils::GetTimezoneOffset()
{
    FTimespan UTCOffset = FDateTime::Now() - FDateTime::UtcNow();
    int32 offset = - FMath::RoundToInt(UTCOffset.GetTotalSeconds() / 60);
    return offset;
}

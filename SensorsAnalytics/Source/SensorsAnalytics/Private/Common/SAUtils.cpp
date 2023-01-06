// Copyright 2022 SensorsData. All Rights Reserved.
#include "SAUtils.h"
#include <iostream>
#include <zlib.h>
#include <sstream>
#include <iomanip>
#include <time.h>

FString FSAUtils::Pattern = TEXT("^((?!^distinct_id$|^original_id$|^time$|^properties$|^id$|^first_id$|^second_id$|^users$|^events$|^event$|^user_id$|^date$|^datetime$|^user_tag.*|^user_group.*)[a-zA-Z_$][a-zA-Z\\d_$]*)$");

FString FSAUtils::EncodeData(const FString& UnprocessedStr)
{
	// Compatible with Chinese
	FTCHARToUTF8 ToUtf8Converter(UnprocessedStr.GetCharArray().GetData());
	auto UnprocessedDataLen = ToUtf8Converter.Length();
	auto UnprocessedData = ToUtf8Converter.Get();
    std::string data = std::string(UnprocessedData);

    std::string compressed_data;
    if (!SACompressString(data, &compressed_data, Z_BEST_COMPRESSION))
    {
        FSALog::Warning(CUR_LOG_POSITION, TEXT("SACompressString Error !"));
    }

    const std::string base64_encoded_data = SABase64Encode(compressed_data);
    std::string request_body = "data_list=" + SAUrlEncode(base64_encoded_data) + "&gzip=1";
    
    FString result = request_body.c_str();
    return result;
    
    
    // Gzip Compress
	int32 CompressBufferLen = FCompression::CompressMemoryBound(NAME_Gzip, UnprocessedDataLen);
	void* CompressBuffer = FMemory::Malloc(CompressBufferLen);
	bool Result = FCompression::CompressMemory(NAME_Gzip, CompressBuffer, CompressBufferLen, UnprocessedData,
		UnprocessedDataLen, ECompressionFlags::COMPRESS_BiasSpeed);

    // Base64
	FString CompressedStr; 
	if ( Result )
	{
		CompressedStr = FBase64::Encode((uint8*)CompressBuffer, CompressBufferLen);
	}
	else
	{
		FSALog::Warning(CUR_LOG_POSITION, TEXT("EncodeData Error !"));
	}
	FMemory::Free(CompressBuffer);
    
    // Create Data
    FString ResultStr = TEXT("data_list=") + CompressedStr + TEXT("&gzip=1");
    FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT(" AddEvent %s"), *ResultStr));
    
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

    FJsonObject::Duplicate(FirstDataObject, SecondDataObject);

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


/**
压缩算法
 **/
bool FSAUtils::SACompressString(const std::string &str,
                                std::string *out_string,
                                int compression_level = Z_BEST_COMPRESSION) {
  z_stream zs;  // z_stream is zlib's control structure
  memset(&zs, 0, sizeof(zs));

  if (deflateInit2(&zs, compression_level, Z_DEFLATED,
                   15 | 16, 8, Z_DEFAULT_STRATEGY) != Z_OK) {
    std::cerr << "deflateInit2 failed while compressing." << std::endl;
    return false;
  }

  zs.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(str.data()));
  zs.avail_in = static_cast<uInt>(str.size());  // set the z_stream's input

  int ret;
  char out_buffer[32768];

  // retrieve the compressed bytes blockwise
  do {
    zs.next_out = reinterpret_cast<Bytef *>(out_buffer);
    zs.avail_out = sizeof(out_buffer);

    ret = deflate(&zs, Z_FINISH);

    if (out_string->size() < zs.total_out) {
      // append the block to the output string
      out_string->append(out_buffer, zs.total_out - out_string->size());
    }
  } while (ret == Z_OK);

  deflateEnd(&zs);

  if (ret != Z_STREAM_END) {  // an error occurred that was not EOF
    std::cerr << "Exception during zlib compression: (" << ret << ") " << zs.msg
              << std::endl;
    return false;
  }

  return true;
}

static const char kBase64Chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string FSAUtils::SABase64Encode(const std::string &data) {
  const unsigned char
      *bytes_to_encode = reinterpret_cast<const unsigned char *>(data.data());
  size_t in_len = data.length();
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len-- > 0) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] =
          ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] =
          ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4); i++)
        ret += kBase64Chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i != 0) {
    for (j = i; j < 3; j++)
      char_array_3[j] = '\0';
    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] =
        ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] =
        ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
    char_array_4[3] = char_array_3[2] & 0x3f;
    for (j = 0; (j < i + 1); j++)
      ret += kBase64Chars[char_array_4[j]];
    while ((i++ < 3))
      ret += '=';
  }
  return ret;
}

std::string FSAUtils::SAUrlEncode(const std::string &data) {
  std::ostringstream escaped;
  escaped.fill('0');
  escaped << std::hex;
  for (std::string::size_type i = 0; i < data.size(); ++i) {
    unsigned char c = data[i];
    // Keep alphanumeric and other accepted characters intact
    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      escaped << c;
      continue;
    }
    // Any other characters are percent-encoded
    escaped << std::uppercase;
    escaped << '%' << std::setw(2) << int((unsigned char) c);
    escaped << std::nouppercase;
  }
  return escaped.str();
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

int FSAUtils::GetTimezoneOffset()
{
    // system time
    time_t _rt = time(NULL);
    // system time -> GMT time
    tm _gtm = *gmtime(&_rt);
    // system time -> local time
    tm _ltm = *localtime(&_rt);
    //    printf("UTC:       %s", asctime(&_gtm));
    //    printf("local:     %s", asctime(&_ltm));
    // GMT time -> system time
    time_t _gt = mktime(&_gtm);
    tm _gtm2 = *localtime(&_gt);
    // 此时 _gt 已经与实际的系统时间 _rt 有时区偏移了，计算两个值的之差就是时区偏的秒数，除以 60 就是分钟
    int offset = - (((_rt - _gt ) + (_gtm2.tm_isdst ? 3600 : 0)) / 60);
    //    printf(" offset (minutes) %d", offset);
    
    return offset;
}

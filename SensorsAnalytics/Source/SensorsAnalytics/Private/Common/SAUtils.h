// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#include "SALog.h"

#include "../Common/SAConstants.h"

#include "SensorsAnalytics.h"
#include "Misc/App.h"
#include "Misc/Compression.h"
#include "Misc/CompressionFlags.h"
#include "Misc/Base64.h"
#include "Misc/DateTime.h"
#include "HAL/UnrealMemory.h"
#include "Internationalization/Regex.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Misc/Guid.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "SocketSubsystem.h"
#include "IAnalyticsProviderET.h"
#include "EngineAnalytics.h"
#include "GeneralProjectSettings.h"
#include "Kismet/KismetTextLibrary.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "Kismet/KismetStringLibrary.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

class FSAUtils 
{
public:

	static bool IsInvalidName(const FString& Name);
    
    static bool IsInvalidValue(const FString& Value);

	static bool CheckProperty(const FString& Properties);

	static uint32 GetScreenWidth();

	static uint32 GetScreenHeight();
    
    static TArray< TSharedPtr<FJsonValue> > GetDataIngestionSource();
    
    static int64 GetTrackID();

    static FString GetGuid();

	static FString GetOS();

	static FString GetOSVersion();

	static int64 GetCurrentTimeStamp();

	static FString GetMachineLoginId();
    
    static FString GetAnonymousID();

    static FString GetUUID();
    
    static FString GetDeviceID();

    static FString GetSerialID();
    
    static FString GetCurrentFormatTime();

	static FString GetEngineVersion();

	static FString GetProjectVersion();
    
	static FString EncodeData(const FString& UnprocessedData);
    
    static FString MergeProperties(const FString& FirstProperty, const FString& SecondProperty);
    
    static FString RemovePropertyWithKey(const FString& Properties, const FString& Key);
        
    static FString AddPluginVersion(const FString& properties, const FString& key, const FString& value);
    
    static int32 GetTimezoneOffset();

private:

 	static FString Pattern;
};


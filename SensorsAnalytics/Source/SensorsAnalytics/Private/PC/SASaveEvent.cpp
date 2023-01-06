// Copyright 2022 SensorsData. All Rights Reserved.
#include "SASaveEvent.h"

USASaveEvent::USASaveEvent()
{
    UserIndex = FSAConstants::SA_SLOT_USER_INDEX_EVENT;
}


void USASaveEvent::AddEvent(TSharedPtr<FJsonObject> EventJson)
{
	FString DataStr;
	TSharedRef<TJsonWriter<>> DataWriter = TJsonWriterFactory<>::Create(&DataStr);
	FJsonSerializer::Serialize(EventJson.ToSharedRef(), DataWriter);

	if ( !EventJsonContent.IsEmpty() )
	{
		EventJsonContent += TEXT("#tad");
	}
	EventJsonContent += DataStr;
    
    FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Add event, current num is: %d"), this->Num()));
}

TArray<TSharedPtr<FJsonObject>> USASaveEvent::GetEvents(uint32 Count)
{
	TArray<TSharedPtr<FJsonObject>> r_Array;
	if ( EventJsonContent.IsEmpty() )
	{
		return r_Array; 
	}

	TArray<FString> TempArray;
	EventJsonContent.ParseIntoArray(TempArray, TEXT("#tad"), false);
    uint32 RealCount = TempArray.Num();
	if (RealCount < Count)
	{
		for (int i = 0; i < TempArray.Num(); i++)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TempArray[i]);
			FJsonSerializer::Deserialize(Reader, JsonObject);
			r_Array.Add(JsonObject);
		}
	}
	else
	{
		for (uint32 i = 0; i < Count; i++)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(TempArray[i]);
			FJsonSerializer::Deserialize(Reader, JsonObject);
			r_Array.Add(JsonObject);
		}
	}

	
	// FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("GetEvents %s"), *EventJsonContent));
	return r_Array;
}

void USASaveEvent::RemoveEvents(uint32 Count)
{
	TArray<FString> TempArray;
	EventJsonContent.ParseIntoArray(TempArray, TEXT("#tad"), false);

	uint32 RealCount = TempArray.Num();
	if (RealCount > Count)
	{
		RealCount = Count;
	}
    if (RealCount > 0)
    {
        TempArray.RemoveAt(0, RealCount, true);
    }

	FString TempContent;
	for (int i = 0; i < TempArray.Num(); i++)
	{
		TempContent += TempArray[i];
		if (i < TempArray.Num() - 1)
		{
			TempContent += TEXT("#tad");
		}
	}
	EventJsonContent = TempContent;
    
    FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Remove events count is: %d, current num is: %d"), Count, this->Num()));
}

uint32 USASaveEvent::Num()
{
	if ( EventJsonContent.IsEmpty() )
	{
		return 0; 
	}
	else
	{
		TArray<FString> TempArray;
		EventJsonContent.ParseIntoArray(TempArray, TEXT("#tad"), false);
		return TempArray.Num();
	}
}

void USASaveEvent::RemoveAllEvents()
{
    this->RemoveEvents(this->Num());
}

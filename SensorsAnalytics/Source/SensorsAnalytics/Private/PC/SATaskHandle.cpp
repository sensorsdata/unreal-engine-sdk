#include "SATaskHandle.h"

bool FSATaskHandle::Init()
{
	FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Init")));
	return true;
}

uint32 FSATaskHandle::Run()
{
	FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Run")));
	while(true)
	{
		FScopeLock SetLock(&SetCritical);
        
		if (IsEnable && !IsPaused && !Working)
		{
#if PLATFORM_WINDOWS
			FWindowsPlatformProcess::Sleep(0.001f);
#else
			FGenericPlatformProcess::Sleep(0.001f);
#endif

			if ( TaskArray.Num() > 0 )
			{
				FString DataStr = TaskArray[0];
                // FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Flush==%s=="), *DataStr));
				if (DataStr.IsEmpty())
				{	
					// FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Flush")));
					Flush();
				}
				else
				{
					TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
					TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(DataStr);
					FJsonSerializer::Deserialize(Reader, JsonObject);
					// FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("SaveToLocal")));
					SaveToLocal(JsonObject);
				}
			}
			else
			{
				IsPaused = true;
			}
		}
	}
	return 0;
}

void FSATaskHandle::Stop()
{
	FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Stop")));
}

void FSATaskHandle::Exit()
{
	FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Exit")));
}

void FSATaskHandle::AddTask(FString EventJsonStr)
{
	FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("Add task with num is %d"), TaskArray.Num()));
	TaskArray.Add(EventJsonStr);
	IsPaused = false;
}

void FSATaskHandle::SetEnableTask(bool Enable)
{
    FScopeLock SetLock(&SetCritical);

    IsEnable = Enable;
}

void FSATaskHandle::ResetWorkingFlag()
{
    FScopeLock SetLock(&SetCritical);
    
    // The Working is reset after the flush fail
    if (IsFlushFail && Working)
    {
        Working = false;
    }
}

FSATaskHandle::FSATaskHandle(USensorsAnalyticsPCBase* Instance)
{
	Working = false;
	IsPaused = true;
    IsFlushFail = false;
	m_Instance = Instance;
	m_Instance->AddToRoot();
    IsEnable = Instance->IsEnableSDK();
	m_SaveName = m_Instance->SlotName + FSAConstants::SA_SLOT_SAVE_EVENT_SUFFIX;

	FScopeLock SetLock(&SetCritical);
	m_SaveEvent = Cast<USASaveEvent>(UGameplayStatics::LoadGameFromSlot(m_SaveName, FSAConstants::SA_SLOT_USER_INDEX_EVENT));

	if ( !m_SaveEvent )
	{
		m_SaveEvent = Cast<USASaveEvent>(UGameplayStatics::CreateSaveGameObject(USASaveEvent::StaticClass()));
		UGameplayStatics::SaveGameToSlot(m_SaveEvent, m_SaveName, FSAConstants::SA_SLOT_USER_INDEX_EVENT);
	}

	m_SaveEvent->AddToRoot();
}

void FSATaskHandle::Flush()
{
	FScopeLock SetLock(&SetCritical);
	if ( !Working )
    {
        Working = true;
        if (TaskArray.Num() > 0)
        {
            TaskArray.RemoveAt(0);
        }
        FlushFromLocalNormal();
    }
}

void FSATaskHandle::SaveToLocal(TSharedPtr<FJsonObject> EventJson)
{
    FSALog::Warning(CUR_LOG_POSITION, TEXT("SaveToLocal !"));

    FScopeLock SetLock(&SetCritical);

	FString InDataStr;
	TSharedRef<TJsonWriter<>> InDataWriter = TJsonWriterFactory<>::Create(&InDataStr);
	FJsonSerializer::Serialize(EventJson.ToSharedRef(), InDataWriter);

	TSharedPtr<FJsonObject> FinalDataObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> FinalReader = TJsonReaderFactory<>::Create(InDataStr);
	FJsonSerializer::Deserialize(FinalReader, FinalDataObject);

    m_SaveEvent->AddEvent(FinalDataObject);
    UGameplayStatics::SaveGameToSlot(m_SaveEvent, m_SaveName, FSAConstants::SA_SLOT_USER_INDEX_EVENT);
    uint32 CurrentNum = m_SaveEvent->Num();
    uint32 FlushBulkSize = m_Instance->sa_GetFlushBulkSize();
    if (CurrentNum >= FlushBulkSize)
    {
        Flush();
    }
    else
    {
        if (TaskArray.Num() > 0)
        {
            TaskArray.RemoveAt(0);
        }
    }
}

void FSATaskHandle::FlushFromLocalNormal()
{
	FScopeLock SetLock(&SetCritical);

	TArray<TSharedPtr<FJsonObject>> SendArray = m_SaveEvent->GetEvents(50);
	if ( SendArray.Num() < 1 )
	{
		Working = false;
    	FSALog::Info(CUR_LOG_POSITION, TEXT("Flush data is Empty"));
		return;
	}
    
	TArray< TSharedPtr<FJsonValue> > DataArray;
	for (int i = 0; i < SendArray.Num(); ++i)
	{
        // add flush time
        TSharedPtr<FJsonObject> SendJsonObject = SendArray[i];
        SendJsonObject->SetNumberField(FSAConstants::SA_EVENT_FLUSH_TIME, FSAUtils::GetCurrentTimeStamp());

		TSharedPtr<FJsonValueObject> DataValue = MakeShareable(new FJsonValueObject(SendJsonObject));
		DataArray.Add(DataValue);
	}

	FString Data;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Data);
	FJsonSerializer::Serialize(DataArray, Writer);
    
    FString ServerUrl = m_Instance->sa_GetServerUrl();
    FSARequestHelper* Helper = new FSARequestHelper();
	Helper->CallHttpRequest(ServerUrl, Data, this, SendArray.Num());
}

void FSATaskHandle::RequestCallback(FString Msg, int32 Code, bool IsSuccess, uint32 EventNum)
{
	FScopeLock SetLock(&SetCritical);
	if ((Code >= 200) && (Code < 300))
	{
        m_SaveEvent->RemoveEvents(EventNum);
        UGameplayStatics::SaveGameToSlot(m_SaveEvent, m_SaveName, FSAConstants::SA_SLOT_USER_INDEX_EVENT);
        FSALog::Warning(CUR_LOG_POSITION, *FString::Printf(TEXT("success = %s , code = %s"), *(UKismetStringLibrary::Conv_BoolToString(IsSuccess)), *FString::FromInt(Code)));
        
		Working = false;
        IsFlushFail = false;
		if ( m_SaveEvent->Num() > 0 )
		{
			Flush();
		}
	}
	else
	{
		FSALog::Error(CUR_LOG_POSITION, *FString::Printf(TEXT("success = %s , code = %s , msg = %s"), *(UKismetStringLibrary::Conv_BoolToString(IsSuccess)), *FString::FromInt(Code), *Msg));
		TaskArray.Insert(TEXT(""), 0);
        
        IsFlushFail = true;
	}
}

void FSATaskHandle::DeleteAllFromLocal()
{
    FScopeLock SetLock(&SetCritical);
    
    m_SaveEvent->RemoveAllEvents();
}

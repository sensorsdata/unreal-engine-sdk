// Copyright 2022 SensorsData. All Rights Reserved.
#include "SARequestHelper.h"

FSARequestHelper::FSARequestHelper()
{
}

void FSARequestHelper::CallHttpRequest(const FString& ServerUrl, const FString& Data, FSATaskHandle* TaskHandle, uint32 EventNum)
{
    FSALog::Warning(CUR_LOG_POSITION, TEXT("\n【Flush Message】\n ServerUrl : ") + ServerUrl + TEXT("\n Data : ") + Data);
    
    m_TaskHandle = TaskHandle;
    m_EventNum = EventNum;
    
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
    Request->SetHeader("Content-Type", "text/plain");

    FString EncodeData = FSAUtils::EncodeData(Data);

    Request->SetContentAsString(EncodeData);
    Request->SetVerb("POST");
    Request->SetTimeout(30);
    Request->SetURL(ServerUrl);

    Request->OnProcessRequestComplete().BindRaw(this, &FSARequestHelper::RequestComplete);
    Request->ProcessRequest();
}

void FSARequestHelper::RequestComplete(FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, bool IsSuccess)
{
    FString Msg = TEXT("Response is nullptr");
    int32 Code = -1;
    if (ResponsePtr != nullptr)
    {
        Msg = ResponsePtr->GetContentAsString();
        Code = ResponsePtr->GetResponseCode();
    }
    
    m_TaskHandle->RequestCallback(Msg, Code, IsSuccess, m_EventNum);
    if ( !EHttpResponseCodes::IsOk(Code) )
    {
        delete this;
    }
}

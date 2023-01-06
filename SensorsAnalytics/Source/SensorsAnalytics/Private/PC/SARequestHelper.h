// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#include "SAEventManager.h"
#include "../Common/SALog.h"
#include "../Common/SAUtils.h"
#include "SATaskHandle.h"

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

class FSARequestHelper
{
public:

	FSARequestHelper();

	void CallHttpRequest(const FString& ServerUrl, const FString& Data, FSATaskHandle* TaskHandle, uint32 EventNum);

private:
	
	FSATaskHandle* m_TaskHandle;

	uint32 m_EventNum;

	void RequestComplete(FHttpRequestPtr RequestPtr, FHttpResponsePtr ResponsePtr, bool IsSuccess);
};

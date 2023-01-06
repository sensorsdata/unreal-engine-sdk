// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#include "SARequestHelper.h"
#include "SensorsAnalyticsPCBase.h"
#include "../Common/SALog.h"
#include "../Common/SAUtils.h"
#include "SASaveEvent.h"
#include "Kismet/KismetStringLibrary.h"

class FSATaskHandle : public FRunnable
{
public:

	FSATaskHandle(USensorsAnalyticsPCBase* Instance);

	virtual bool Init() override;

	virtual uint32 Run() override;

	virtual void Stop() override;

	virtual void Exit() override;

	void AddTask(FString EventJsonStr);

	void RequestCallback(FString Msg, int32 Code, bool IsSuccess, uint32 EventNum);
    
    void DeleteAllFromLocal();
    
    void SetEnableTask(bool Enable);
    
    void ResetWorkingFlag();

private:

    USensorsAnalyticsPCBase* m_Instance;

	TArray<FString> TaskArray;

	FCriticalSection SetCritical;

	USASaveEvent* m_SaveEvent;
	
	FString m_SaveName;

	bool Working;

	bool IsPaused;

    bool IsEnable;
    
    bool IsFlushFail;
    
	void Flush();

	void SaveToLocal(TSharedPtr<FJsonObject> EventJson);

	void FlushFromLocalNormal();
};

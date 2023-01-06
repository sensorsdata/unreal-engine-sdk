// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#include "SAEventManager.generated.h"

static TMap<FString, TArray<TSharedPtr<FJsonObject>>> TAEventSendMap;

//用于编译时循环引用错误
class FSATaskHandle;

class USASaveEvent;

class USensorsAnalyticsPCBase;

UENUM()
enum class SAEventType : uint8
{
    TRACK = 0,
    TRACK_SIGNUP = 1,
    PROFILE_SET = 2,
    PROFILE_SET_ONCE = 3,
    PROFILE_UNSET = 4,
    PROFILE_DELETE = 5,
    PROFILE_APPEND = 6,
    PROFILE_INCREMENT = 7
};

UCLASS()
class USAEventManager : public UObject
{

GENERATED_BODY()

public:

	USAEventManager();

	void EnqueueUserEvent(const FString& EventType, const FString& Properties);

    void EnqueueTrackEvent(const FString& EventName, const FString& Properties, SAEventType type);

	void Flush();

	void BindInstance(USensorsAnalyticsPCBase *Instance);
    
    void DeleteAllEvents();
    
    void SetEnableManager(bool Enable);

private:

	~USAEventManager();

	UGameInstance* GetGameInstance();

	FSATaskHandle* m_TaskHandle;

	UGameInstance* m_GameInstance;

	FRunnableThread* m_RunnableThread;

    USensorsAnalyticsPCBase* m_Instance;

	FTimerHandle WorkHandle;
};

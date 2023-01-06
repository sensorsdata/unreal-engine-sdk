// Copyright 2022 SensorsData. All Rights Reserved.
#pragma once

#include "../Common/SALog.h"
#include "../Common/SAConstants.h"

#include "GameFramework/SaveGame.h"
#include "SASaveEvent.generated.h"

UCLASS()
class USASaveEvent : public USaveGame
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString EventJsonContent;

    UPROPERTY(VisibleAnywhere, Category = Basic)
    uint32 UserIndex;

	USASaveEvent();

    void AddEvent(TSharedPtr<FJsonObject> EventJson);

    TArray<TSharedPtr<FJsonObject>> GetEvents(uint32 Count);

    void RemoveEvents(uint32 Count);
    
    void RemoveAllEvents();

    uint32 Num();
};

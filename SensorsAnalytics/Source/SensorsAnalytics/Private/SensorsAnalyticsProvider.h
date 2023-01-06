//
//  SensorsAnalyticsProvider.h
//  FirstGame
//
//  Created by MC on 2022/11/28.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#pragma once
#include "Interfaces/IAnalyticsProvider.h"

class FSensorAnalyticsProvider : public IAnalyticsProvider
{
    bool bHasSessionStarted;

    static TSharedPtr<IAnalyticsProvider> Provider;
    FSensorAnalyticsProvider();

public:

    static TSharedPtr<IAnalyticsProvider> Create()

    {
        if (!Provider.IsValid())
        {
            Provider = TSharedPtr<IAnalyticsProvider>(new FSensorAnalyticsProvider());
        }
        return Provider;
    }

    static void Destroy()
    {
          Provider.Reset();
    }

    static TSharedPtr<FSensorAnalyticsProvider> GetProvider()
    {
        return StaticCastSharedPtr<FSensorAnalyticsProvider>(Provider);
    }

    virtual ~FSensorAnalyticsProvider();

    virtual bool StartSession(const TArray<FAnalyticsEventAttribute>& Attributes) override;

    virtual void EndSession() override;

    virtual void FlushEvents() override;

    virtual void SetUserID(const FString& InUserID) override;

    virtual FString GetUserID() const override;

    virtual FString GetSessionID() const override;

    virtual bool SetSessionID(const FString& InSessionID) override;

    virtual void RecordEvent(const FString& EventName, const TArray<FAnalyticsEventAttribute>& Attributes) override;

    virtual void RecordItemPurchase(const FString& ItemId, const FString& Currency, int PerItemCost, int ItemQuantity) override;

    virtual void RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const FString& RealCurrencyType, float RealMoneyCost, const FString& PaymentProvider) override;

    virtual void RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount) override;

    virtual void SetBuildInfo(const FString& InBuildInfo) override;

    virtual void SetGender(const FString& InGender) override;

    virtual void SetLocation(const FString& InLocation) override;

    virtual void SetAge(const int32 InAge) override;

    virtual void RecordItemPurchase(const FString& ItemId, int ItemQuantity, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;

    virtual void RecordCurrencyPurchase(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;

    virtual void RecordCurrencyGiven(const FString& GameCurrencyType, int GameCurrencyAmount, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;

    virtual void RecordError(const FString& Error, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;

    virtual void RecordProgress(const FString& ProgressType, const FString& ProgressHierarchy, const TArray<FAnalyticsEventAttribute>& EventAttrs) override;
};


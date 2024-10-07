// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WebBrowser.h"
#include "Nice1WebLoginWidget.generated.h"

class UNice1WebLoginHandler;

/**
 * UWebBrowser derived class used for login 
 */
UCLASS(/*HideCategories = (Appearance)*/)
class NICE1PLUGIN_API UNice1WebLoginWidget : public UWebBrowser
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginSuccess, const FString&, Account);

	/* Called after successful web login */
	UPROPERTY(BlueprintAssignable, Category = "Nice1")
	FOnLoginSuccess OnLoginSuccess;

protected:
	UNice1WebLoginHandler* WebLoginHandler;

protected:
	UFUNCTION()
	void HandleOnLoginSuccess(const FString& Account);

	UFUNCTION()
	void LogWebConsoleMessage(const FString& Message, const FString& Source, int32 Line);

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface
};

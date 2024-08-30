// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Nice1WebLoginHandler.generated.h"

/**
 * Simple class used for JavaScript communication. Public methods will be exposed to JS when bound to a SWebBrowser
 */
UCLASS()
class NICE1PLUGIN_API UNice1WebLoginHandler : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginSuccess, const FString&, Account);

	UPROPERTY(BlueprintAssignable, Category = "Nice1")
	FOnLoginSuccess OnLoginSuccess;

public:
	/* Called in JavaScript after successful login */
	UFUNCTION()
	void NotifyLogin(const FString& Account);
};

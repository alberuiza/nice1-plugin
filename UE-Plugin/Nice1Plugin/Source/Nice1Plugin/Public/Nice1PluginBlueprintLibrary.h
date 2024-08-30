// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PluginBlueprintLibrary.h"
#include "Nice1PluginBlueprintLibrary.generated.h"

/**
 * Nice1 Blueprint library exposing functions from the Nice1 DLL API
 */
UCLASS()
class NICE1PLUGIN_API UNice1PluginBlueprintLibrary : public UPluginBlueprintLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "Nice1")
	static FString Nice1_CheckLicensePlugin(const FString& Owner);
	
	UFUNCTION(BlueprintCallable, Category="Nice1")
	static int Nice1_APITest();
};

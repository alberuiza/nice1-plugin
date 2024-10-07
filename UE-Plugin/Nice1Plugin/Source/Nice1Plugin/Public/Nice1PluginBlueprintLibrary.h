// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Nice1PluginBlueprintLibrary.generated.h"

UENUM(BlueprintType)
enum class FNICE1PluginStatus : uint8
{
    NONE                UMETA(DisplayName = "None"),
    NICE1GENESISKEY     UMETA(DisplayName = "Nice1GenesisKey"),
    LICENSED            UMETA(DisplayName = "Licensed"),
    NO_LICENSE          UMETA(DisplayName = "No license"),
    LICENSE_ERROR       UMETA(DisplayName = "Error")
};

/**
 * Nice1 Blueprint library exposing functions from the Nice1 DLL API
 */
UCLASS()
class NICE1PLUGIN_API UNice1PluginBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
    /**
     * Checks the license status for a specified account.
     *
     * @param AccountID The identifier of the accout whose license is being checked.
     * @return A string describing the license status.
     */
	UFUNCTION(BlueprintCallable, Category = "Nice1")
	static const FNICE1PluginStatus& CheckLicense(const FString& AccountID);

    /**
     * Retrieves the current license status of the plugin.
     *
     * @return A reference to the current plugin license status as an FNICE1PluginStatus enum.
     */
    UFUNCTION(BlueprintCallable, Category = "Nice1")
    static const FNICE1PluginStatus& GetLicenseStatus();

    /**
     * Checks if the logged-in user has a valid license.
     * @return true if the logged-in user is licensed, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Nice1")
    static bool IsLicensed();

private:
    static FNICE1PluginStatus Status;
};

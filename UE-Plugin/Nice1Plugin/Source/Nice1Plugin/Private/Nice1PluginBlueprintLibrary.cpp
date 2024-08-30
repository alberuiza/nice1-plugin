// Fill out your copyright notice in the Description page of Project Settings.


#include "Nice1PluginBlueprintLibrary.h"
#include "ThirdParty/Nice1/include/Plugin.h"

#include "Nice1PluginSettings.h"

FString UNice1PluginBlueprintLibrary::Nice1_CheckLicensePlugin(const FString& Owner)
{
	if(!ensureMsgf(!Owner.IsEmpty(), TEXT("Owner was empty. Please pass a non-empty owner FString.")))
	{
		return TEXT("");
	}

	const UNice1PluginSettings* Settings = GetDefault<UNice1PluginSettings>();
	if (!ensureMsgf(Settings != nullptr, TEXT("Could not get Nice1Plugin settings")))
	{
		return TEXT("");
	}

	if (!ensureMsgf(!Settings->AUTHOR.IsEmpty() && !Settings->CATEGORY.IsEmpty() && !Settings->IDATA_NAME.IsEmpty(), 
		TEXT("One or more fields in Nice1 Plugin Settings was missing. Please fill out all information under Project Settings -> Nice1 Settings")))
	{
		return TEXT("");
	}

	uint8 NetworkIndex = (uint8)Settings->Network;
	char* Owner_char_ptr = TCHAR_TO_ANSI(*Owner);
	char* Author = TCHAR_TO_ANSI(*Settings->AUTHOR);
	char* Category = TCHAR_TO_ANSI(*Settings->CATEGORY);
	char* LicenseName = TCHAR_TO_ANSI(*Settings->IDATA_NAME);
	bool bUseNice1GenesisKey = Settings->UseNice1GenesisKey;

	FString result = FString(CheckLicensePlugin(Owner_char_ptr, Author, Category, LicenseName, bUseNice1GenesisKey, NetworkIndex));

	UE_LOG(LogTemp, Display, TEXT("Nice1_CheckLicensePlugin -> result was: %s"), *result);

	return result;
}

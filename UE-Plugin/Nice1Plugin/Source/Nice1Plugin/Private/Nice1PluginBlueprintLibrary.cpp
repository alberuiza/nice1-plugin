// Fill out your copyright notice in the Description page of Project Settings.


#include "Nice1PluginBlueprintLibrary.h"
#include "ThirdParty/Nice1/include/Plugin.h"

#include "Nice1PluginSettings.h"

FNICE1PluginStatus UNice1PluginBlueprintLibrary::Status = FNICE1PluginStatus::NONE;

/*static*/ const FNICE1PluginStatus& UNice1PluginBlueprintLibrary::CheckLicense(const FString& AccountID)
{
	if(!ensureMsgf(!AccountID.IsEmpty(), TEXT("AccountID was empty. Please pass a non-empty owner FString.")))
	{
		return Status = FNICE1PluginStatus::LICENSE_ERROR;
	}

	const UNice1PluginSettings* Settings = GetDefault<UNice1PluginSettings>();
	if (!ensureMsgf(Settings != nullptr, TEXT("Could not get Nice1Plugin settings")))
	{
		return Status = FNICE1PluginStatus::LICENSE_ERROR;
	}

	if (!ensureMsgf(!Settings->AUTHOR.IsEmpty() && !Settings->CATEGORY.IsEmpty() && !Settings->IDATA_NAME.IsEmpty(), 
		TEXT("One or more fields in Nice1 Plugin Settings was missing. Please fill out all information under Project Settings -> Nice1 Settings")))
	{
		return Status = FNICE1PluginStatus::LICENSE_ERROR;
	}

	uint8 NetworkIndex = (uint8)Settings->Network;
	char* Owner_char_ptr = TCHAR_TO_ANSI(*AccountID);
	char* Author = TCHAR_TO_ANSI(*Settings->AUTHOR);
	char* Category = TCHAR_TO_ANSI(*Settings->CATEGORY);
	char* LicenseName = TCHAR_TO_ANSI(*Settings->IDATA_NAME);
	bool bUseNice1GenesisKey = Settings->UseNice1GenesisKey;
	FString result = FString(CheckLicensePlugin(Owner_char_ptr, Author, Category, LicenseName, bUseNice1GenesisKey, NetworkIndex));

	UE_LOG(LogTemp, Display, TEXT("CheckLicense -> result was: %s"), *result);

	if (result.Equals(TEXT("NICE1GENESISKEY"), ESearchCase::IgnoreCase))
	{
		Status = FNICE1PluginStatus::NICE1GENESISKEY;
	}
	else if (result.Equals(TEXT("LICENSE"), ESearchCase::IgnoreCase))
	{
		Status = FNICE1PluginStatus::LICENSED;
	}
	else if (result.Equals(TEXT("NO_LICENSE"), ESearchCase::IgnoreCase))
	{
		Status = FNICE1PluginStatus::NO_LICENSE;
	}
	else if (result.Equals(TEXT("ERROR"), ESearchCase::IgnoreCase))
	{
		Status = FNICE1PluginStatus::LICENSE_ERROR;
	}
	else
	{
		Status = FNICE1PluginStatus::NONE;
	}

	return Status;
}

/*static*/ const FNICE1PluginStatus& UNice1PluginBlueprintLibrary::GetLicenseStatus()
{
	return Status;
}

/*static*/ bool UNice1PluginBlueprintLibrary::IsLicensed()
{
	return Status == FNICE1PluginStatus::LICENSED || Status == FNICE1PluginStatus::NICE1GENESISKEY;
}

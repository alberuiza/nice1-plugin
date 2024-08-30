// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DeveloperSettings.h"
#include "Nice1PluginSettings.generated.h"

UENUM(BlueprintType)
enum class ENice1Network : uint8
{
	Jungle4        =0  UMETA(DisplayName = "Jungle4"),
	Eos            =1  UMETA(DisplayName = "Eos"),
	Proton_testnet =2  UMETA(DisplayName = "Proton_testnet"),
	Proton         =3  UMETA(DisplayName = "Proton"),
	Wax_testnet    =4  UMETA(DisplayName = "Wax_testnet"),
	Wax            =5  UMETA(DisplayName = "Wax"),
	Telos_testnet  =6  UMETA(DisplayName = "Telos_testnet"),
	Telos          =7  UMETA(DisplayName = "Telos")
};

/**
 * Developer settings for Nice1 Plugin
 */
UCLASS(Config = Nice1Settings, meta = (DisplayName = "Nice1 Settings"))
class NICE1PLUGIN_API UNice1PluginSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Nice1 Settings")
	ENice1Network Network;

	UPROPERTY(Config, EditAnywhere, Category = "Nice1 Settings")
	bool UseNice1GenesisKey;

	UPROPERTY(Config, EditAnywhere, Category = "Nice1 Settings")
	bool UseFreeToPlayLicense;
	
	UPROPERTY(Config, EditAnywhere, Category = "Nice1 Settings|License data")
	FString AUTHOR;

	UPROPERTY(Config, EditAnywhere, Category = "Nice1 Settings|License data")
	FString IDATA_NAME;

	UPROPERTY(Config, EditAnywhere, Category = "Nice1 Settings|License data")
	FString CATEGORY;
};

// Copyright Epic Games, Inc. All Rights Reserved.

#include "Nice1Plugin.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FNice1PluginModule"

void FNice1PluginModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FString BaseDir = IPluginManager::Get().FindPlugin("Nice1Plugin")->GetBaseDir();
 
 	// Add on the relative location of the third party dll and load it
 	FString Nice1PluginDLLPath;
 #if PLATFORM_WINDOWS
 	Nice1PluginDLLPath = FPaths::Combine(*BaseDir, TEXT("Source/ThirdParty/Nice1/lib/Win64/Release/Nice1Plugin.dll"));
 #endif
 
 	DLLHandle = !Nice1PluginDLLPath.IsEmpty() ? FPlatformProcess::GetDllHandle(*Nice1PluginDLLPath) : nullptr;
}

void FNice1PluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	FPlatformProcess::FreeDllHandle(DLLHandle);
	DLLHandle = nullptr;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNice1PluginModule, Nice1Plugin)
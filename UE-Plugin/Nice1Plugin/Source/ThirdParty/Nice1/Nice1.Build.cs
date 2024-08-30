// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class Nice1 : ModuleRules
{
	public Nice1(ReadOnlyTargetRules Target) : base(Target)
	{
		Type = ModuleType.External;

        PublicSystemIncludePaths.Add(Path.Combine(ModuleDirectory, "include"));

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            // Add the import library
             PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "lib", "Win64", "Release", "Nice1Plugin.lib"));

            // Ensure that the DLL is staged along with the executable
            RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/Nice1/lib/Win64/Release/Nice1Plugin.dll");

            // Copy dll
            RuntimeDependencies.Add("$(TargetOutputDir)/NicePlugin.dll", Path.Combine(PluginDirectory, "Source/ThirdParty/Nice1/lib/Win64/Release/Nice1Plugin.dll"));

            PublicDelayLoadDLLs.Add("Nice1Plugin.dll");
        }
        else if(Target.Platform == UnrealTargetPlatform.Linux)
        {
            // ...
        }
        else if (Target.Platform == UnrealTargetPlatform.Mac)
        {
            // ...
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            // ...
        }
        else if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            // ...
        }
    }
}

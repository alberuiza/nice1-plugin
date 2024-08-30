// Copyright Epic Games, Inc. All Rights Reserved.
using System.IO;
using UnrealBuildTool;

public class Nice1Plugin : ModuleRules
{
	public Nice1Plugin(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {"Core", "WebBrowser","WebBrowserWidget"});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "Projects",
                "Slate",
                "SlateCore",
                "UMG",
                "DeveloperSettings",
                "Nice1"
			}
		);
	}
}

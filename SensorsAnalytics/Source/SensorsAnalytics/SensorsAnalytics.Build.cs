// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

namespace UnrealBuildTool.Rules
{
public class SensorsAnalytics : ModuleRules
{
	public SensorsAnalytics(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);

        PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Private")));
        PrivateIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));
        PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "Public")));
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "HTTP",
                "Json",
                "JsonUtilities",
                "Sockets",
                "EngineSettings"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "Analytics",
                "CoreUObject",
                "Json",
                "Engine",
                "Projects",
                "HTTP",
                "Json",
                "JsonUtilities",
                "Sockets",
                "EngineSettings"
			}
			);

            if (Target.Type == TargetRules.TargetType.Editor)
            {
                PrivateDependencyModuleNames.Add("UnrealEd");
            }

            PrivateIncludePathModuleNames.AddRange(
                new string[]
                {
                    "Settings",
                    "Projects"
                }
                );

            if (Target.Platform == UnrealTargetPlatform.IOS)
            {
                PublicAdditionalFrameworks.Add(
                    new Framework(
                        "SensorsAnalyticsSDK",
                        "../ThirdParty/iOS/SensorsAnalyticsSDK.embeddedframework.zip",
                        "Resources/SensorsAnalyticsSDK.bundle"
                    )
                );
            }
            else if(Target.Platform == UnrealTargetPlatform.Android)
            {
                // Unreal Plugin Language
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add("AndroidPlugin", System.IO.Path.Combine(PluginPath, "SensorsAnalytics_UPL.xml"));
                // JNI
                PublicIncludePathModuleNames.Add("Launch");
            }
            else if(Target.Platform == UnrealTargetPlatform.Win64)
            {
                // add header
                PublicIncludePaths.Add(Path.GetFullPath(Path.Combine(ModuleDirectory, "../ThirdParty/Windows/zlib_x64/Include")));
                // add lib
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../ThirdParty/Windows/zlib_x64/lib/libz-static.lib"));
            }

        DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

        // add zlib library
        // public bool LoadZlib(TargetInfo Target)
        // {
        //    bool isLibararySupported = false;
        //    if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        //    {
        //        isLibararySupported = true;
                // string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "Win64" : "Win32";
                // PublicAdditionalLibraries.Add(Path.Combine(LibraryPath, PlatformString + ".lib"));
                // PublicDelayLoadDLLs.Add(PlatformString + ".dll");
                // RuntimeDependencies.Add(new RuntimeDependency(LibraryPath + PlatformString + ".dll"));
        //       PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../ThirdParty/Windows/zlib_x64/lib/libz-static.lib"));
        //   }
        //   return isLibararySupported;
        // }
	}
}
}

// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.IO;
public class BoatSim : ModuleRules
{
	public BoatSim(ReadOnlyTargetRules Target) : base(Target)
	{
		OptimizeCode = CodeOptimization.Never;
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        PublicDependencyModuleNames.AddRange(new string[] { "Core", "RenderCore", "CoreUObject", "Engine", "InputCore", "GeoReferencing", "Networking","Sockets","ProceduralMeshComponent","UMG","Slate","SlateCore","RHI","Json", "Niagara", "ImageWrapper"});

		PrivateDependencyModuleNames.AddRange(new string[] {  });


        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            const string GStreamerRoot = @"C:\gstreamer\1.0\msvc_x86_64"; // path to gstreamer development package

            PrivateIncludePaths.Add(Path.Combine(GStreamerRoot, "include"));
            PrivateIncludePaths.Add(Path.Combine(GStreamerRoot, "include", "gstreamer-1.0"));
            PrivateIncludePaths.Add(Path.Combine(GStreamerRoot, "include", "glib-2.0"));
            PrivateIncludePaths.Add(Path.Combine(GStreamerRoot, "lib", "glib-2.0", "include"));

            var GStreamerLibPath = Path.Combine(GStreamerRoot, "lib");
            PublicSystemLibraryPaths.Add(GStreamerLibPath);

            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "glib-2.0.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "gobject-2.0.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "gstreamer-1.0.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "gstvideo-1.0.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(GStreamerLibPath, "gstapp-1.0.lib"));

            PublicDelayLoadDLLs.Add("glib-2.0-0.dll");
            PublicDelayLoadDLLs.Add("gobject-2.0-0.dll");
            PublicDelayLoadDLLs.Add("gstreamer-1.0-0.dll");
            PublicDelayLoadDLLs.Add("gstvideo-1.0-0.dll");
            PublicDelayLoadDLLs.Add("gstapp-1.0-0.dll");
        }
    }
}

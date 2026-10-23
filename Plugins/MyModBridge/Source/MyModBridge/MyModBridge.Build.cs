// in <Project or Plugin> .Build.cs
using UnrealBuildTool;
using System.IO;

public class MyModBridge : ModuleRules
{
    public MyModBridge(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new[] { "Core", "CoreUObject", "Engine" });

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDelayLoadDLLs.Add("MyMod.dll");

            // Project module example:
            string ProjectDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
            string DllSource = Path.Combine(ProjectDir, "Binaries", "Win64", "MyMod.dll");

            // Plugin module example:
            // string PluginDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
            // string DllSource = Path.Combine(PluginDir, "ThirdParty", "MyMod", "Win64", "MyMod.dll");

            if (File.Exists(DllSource))
            {
                RuntimeDependencies.Add("$(TargetOutputDir)/MyMod.dll", DllSource);
            }
            else
            {
                System.Console.WriteLine("WARNING: MyMod.dll not found at " + DllSource + " - skipping staging.");
            }
        }
    }
}
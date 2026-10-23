using System.IO;
using UnrealBuildTool;

public class MyDLLDemo : ModuleRules
{
    public MyDLLDemo(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] { "Core", "CoreUObject", "Engine", "InputCore" }
        );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            PublicDelayLoadDLLs.Add("MyMod.dll");

            string ProjectDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
            string DllSource = Path.Combine(ProjectDir, "Binaries", "Win64", "MyMod.dll");

            // Adds the DLL as a runtime dependency so Unreal packages/copies it correctly
            RuntimeDependencies.Add(DllSource);
        }
    }
}
using UnrealBuildTool;

public class BuildToolSystem : ModuleRules {
    public BuildToolSystem(ReadOnlyTargetRules target) : base(target) {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "CoreUObject", "Engine",
            "Slate", "SlateCore", "UMG",
            "CommonUI", "InputCore",
            "DeveloperSettings",

            "BlueprintUtilities",
            "WidgetUtilities",
        });
    }
}

using UnrealBuildTool;

public class Grimward : ModuleRules
{
    public Grimward(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        RegisterGameDependencies();
        RegisterIncludePaths();

        if (Target.bBuildEditor)
        {
            RegisterEditorDependencies();
        }
    }
    public void RegisterGameDependencies()
    {
        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            
        });
    }
    public void RegisterEditorDependencies()
    {
        PublicDependencyModuleNames.AddRange(new string[]
        {

        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
                "UnrealEd",
                "Slate",
                "SlateCore",
                "EditorFramework",
                "LevelEditor"
        });
    }
    public void RegisterIncludePaths()
    {
        PublicIncludePaths.AddRange(new string[]
        {
            "Grimward/Public",
            "Grimward/Public/Data",
            "Grimward/Public/Game",
        });

        PrivateIncludePaths.AddRange(new string[]
        {
            "Grimward/Private",
            "Grimward/Private/Data",
            "Grimward/Private/Game",
        });
    }
}
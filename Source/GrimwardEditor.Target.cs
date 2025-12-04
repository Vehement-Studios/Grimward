// Copyright Vehement Studios

using UnrealBuildTool;
using System.Collections.Generic;

public class GrimwardTarget : TargetRules
{
    public GrimwardTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.Add("Grimward");
    }
}
using UnrealBuildTool;

public class CTRLCoreEditor : ModuleRules
{
	public CTRLCoreEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DeveloperSettings",
				"EditorSubsystem",
			}
		);

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"ContentBrowserData",
				"CoreUObject",
				"CTRLCore",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
			}
		);
	}
}
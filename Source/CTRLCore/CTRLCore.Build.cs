// Some copyright should be here...

using UnrealBuildTool;

public class CTRLCore : ModuleRules
{
	public CTRLCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		IWYUSupport = IWYUSupport.Full;

		PublicDependencyModuleNames.AddRange([
			"Core",
			"DeveloperSettings",
			"Engine",
		]);


		PrivateDependencyModuleNames.AddRange([
			"AIModule",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore"
		]);

		if (Target.bBuildEditor)
		{
			// Needed for UCTRLWorldSubsystemBlueprintLoader loading BP subsystems before PIE
			PrivateDependencyModuleNames.Add("EditorFramework");
			PrivateDependencyModuleNames.Add("UnrealEd");
		}
	}
}
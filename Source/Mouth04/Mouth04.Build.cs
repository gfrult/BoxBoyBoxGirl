// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Mouth04 : ModuleRules
{
	public Mouth04(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","Paper2D","PhysicsCore","UMG","Slate","SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		// ========== 新增的PublicIncludePaths配置 ==========
		PublicIncludePaths.AddRange(new string[] 
		{
			"Mouth04/Public/UI",
			// 可添加多个路径，比如还有Login相关的UI："Mouth04/Public/Login/UI",
		});
		
		
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

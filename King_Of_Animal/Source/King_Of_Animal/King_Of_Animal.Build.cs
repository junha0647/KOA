// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class King_Of_Animal : ModuleRules
{
	public King_Of_Animal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}

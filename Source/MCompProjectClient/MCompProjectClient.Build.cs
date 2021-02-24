// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MCompProjectClient : ModuleRules
{
	public MCompProjectClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}

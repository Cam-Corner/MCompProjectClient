// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MCompProjectClient : ModuleRules
{
	public MCompProjectClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		//PCHUsage = PCHUsageMode.UseSharedPCHs;

		//PrivatePCHHeaderFile = "Public/Networking/NetworkedGame.h";

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MCompProjectServerTarget : TargetRules
{
	public MCompProjectServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bCompileChaos = true;
		bUseChaos = true;

		ExtraModuleNames.Add("MCompProjectClient");
	}
}

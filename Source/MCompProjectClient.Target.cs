// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MCompProjectClientTarget : TargetRules
{
	public MCompProjectClientTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		bCompileChaos = true;
		bUseChaos = true;

		ExtraModuleNames.Add("MCompProjectClient");
	}
}

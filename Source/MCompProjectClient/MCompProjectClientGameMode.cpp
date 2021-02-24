// Copyright Epic Games, Inc. All Rights Reserved.

#include "MCompProjectClientGameMode.h"
#include "MCompProjectClientCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMCompProjectClientGameMode::AMCompProjectClientGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

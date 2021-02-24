// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/MPGameMode.h"
#include "Networking/MPPlayerController.h"
#include "Networking/MPPlayerState.h"
#include "Networking/MPGameState.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AMPGameMode::AMPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnOB(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	DefaultPawnClass = PlayerPawnOB.Class;

	PlayerControllerClass = AMPPlayerController::StaticClass();
	PlayerStateClass = AMPPlayerState::StaticClass();
	GameStateClass = AMPGameState::StaticClass();
}

bool AMPGameMode::ReadyToStartMatch_Implementation()
{
	Super::ReadyToEndMatch();

	return _MaxNumberOfPlayers == NumPlayers;
}

void AMPGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	_PlayerControllers.Add(NewPlayer);

	//NewPlayer->PlayerState->SetPlayerName("Unknown Name");

	AMPPlayerController* MPController = Cast<AMPPlayerController>(NewPlayer);

	if (MPController)
	{
		MPController->AskClientToSetName();
		UE_LOG(LogTemp, Display, TEXT("Asking client for display name..."));
	}
	else
		UE_LOG(LogTemp, Error, TEXT("Using wrong player controller!"));
}

void AMPGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);




}

void AMPGameMode::BeginPlay()
{
	Super::BeginPlay();

	_MaxNumberOfPlayers = FCString::Atoi(*(UGameplayStatics::ParseOption(OptionsString, "_MaxNumberOfPlayers")));
}



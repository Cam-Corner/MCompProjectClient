// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/MPGameMode.h"
#include "Networking/MPGameState.h"
#include "Networking/MPPlayerController.h"
#include "Networking/MPPlayerState.h"
#include "Networking/MPCharacter.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"
#include "GameCamera.h"
#include "Kismet/GameplayStatics.h"

AMPGameMode::AMPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnOB(TEXT("/Game/Blueprints/bp_Character"));
	DefaultPawnClass = PlayerPawnOB.Class; //AMPCharacter::StaticClass();

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

void AMPGameMode::RestartPlayer(AController* Player)
{
	Super::RestartPlayer(Player);
	UE_LOG(LogTemp, Warning, TEXT("Restarting Player!"));
	AMPPlayerController* PC = Cast<AMPPlayerController>(Player);
	if (PC != NULL)
	{
		PC->SetClientsCamera();
		UE_LOG(LogTemp, Warning, TEXT("Sent Request for clients to change their camera to the GameCamera!"));
	}
}

void AMPGameMode::PlayerAttackedPlayer(AActor* AttackerActoR, AController* AttackerC, 
	AActor* DamagedActor, AController* DamagedC, FHitInfo HitInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("Player Damaged a Player"));
	
	AMPCharacter* MPC = Cast<AMPCharacter>(DamagedActor);

	if (MPC != NULL)
	{
		MPC->RemoveSomeHP(10, DamagedC->PlayerState->GetPlayerName());
	}
}

//void AMPGameMode::SendChatMessageToAllClients(const FChatMessage ChatMessage)
//{
//	for (APlayerController* PC : _PlayerControllers)
//	{
//		AMPPlayerController* MPPC = Cast<AMPPlayerController>(PC);
//		if (MPPC != NULL)
//		{
//			MPPC->ClientReceiveNewChatMessage(ChatMessage);
//		}
//		else
//		{
//			UE_LOG(LogTemp, Error, TEXT("Cannot cast PC to MPPC"));
//		}
//	}
//}
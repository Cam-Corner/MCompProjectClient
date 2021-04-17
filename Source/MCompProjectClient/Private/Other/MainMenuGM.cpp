// Fill out your copyright notice in the Description page of Project Settings.


#include "Other/MainMenuGM.h"
#include "UI/MainMenuHUD.h"
#include "Other/MainMenuPC.h"
#include "Other/CharacterCustomiserActor.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Networking/MPGameMode.h"


AMainMenuGM::AMainMenuGM()
{
	//In the main menu so we dont need a pawn
	DefaultPawnClass = NULL; 

	//Need to set the main menu HUD
	HUDClass = AMainMenuHUD::StaticClass();
	
	//Menu doesnt need much functionality so we made a basic PC just for the menu
	PlayerControllerClass = AMainMenuPC::StaticClass();
	
	//Dont need a player state for the menu
	//PlayerStateClass = AMPPlayerState::StaticClass();
	
	//Dont need a gamestate for the main menu
	//GameStateClass = AMPGameState::StaticClass();
}

void AMainMenuGM::BeginPlay()
{
	Super::BeginPlay();

#if WITH_SERVER_CODE

	 //GetWorld()->ServerTravel("World'/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap.ThirdPersonExampleMap' ?listen ?game = AMPGameMode");

#endif

	//Spawn the character customiser
	if (APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0))
	{
		FVector SpawnLocation = PCM->GetCameraLocation();

		_CCActor = GetWorld()->SpawnActor<ACharacterCustomiserActor>();
		
		if (_CCActor != NULL)
		{
			_CCActor->SetActorLocation(SpawnLocation);
		}
	}
}

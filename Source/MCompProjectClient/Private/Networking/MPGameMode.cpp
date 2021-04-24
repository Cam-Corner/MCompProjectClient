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
#include "UI/MainMenuHUD.h"
#include "GameFramework/PlayerStart.h"
#include "UI/GameHUD.h"
#include "Networking/MPCamera.h"

DEFINE_LOG_CATEGORY(LogAMPGameMode);

AMPGameMode::AMPGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnOB(TEXT("/Game/Blueprints/bp_Character"));
	DefaultPawnClass = PlayerPawnOB.Class; //AMPCharacter::StaticClass();
	//HUDClass = AMainMenuHUD::StaticClass();
	HUDClass = AGameHUD::StaticClass();
	PlayerControllerClass = AMPPlayerController::StaticClass();
	PlayerStateClass = AMPPlayerState::StaticClass();
	GameStateClass = AMPGameState::StaticClass();

	_bGameCanStart = false;
}

bool AMPGameMode::ReadyToStartMatch_Implementation()
{
	Super::ReadyToEndMatch();

	if (!_bGameCanStart)
		return false;

	return true;
	//return _MaxNumberOfPlayers == NumPlayers;
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
		UE_LOG(LogAMPGameMode, Display, TEXT("Asking client for display name..."));
	}
	else
		UE_LOG(LogAMPGameMode, Error, TEXT("Using wrong player controller!"));

	if (_PlayerControllers.Num() >= 1 && GetMatchState() == MatchState::WaitingToStart)
	{
		//_bGameStarted = true;
		//StartMatch();
		_bGameCanStart = true;
		MPController->SetClientsCamera();
	}
}

void AMPGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	if (GetMatchState() == MatchState::InProgress)
	{
		if (AMPPlayerController* MPPC = Cast<AMPPlayerController>(NewPlayer))
		{
			MPPC->SetClientsCamera();
			//SpawnPlayerCamera(NewPlayer);
		}
	}
}

void AMPGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	for (APlayerController* PC : _PlayerControllers)
	{
		if (AMPPlayerController* MPPC = Cast<AMPPlayerController>(PC))
			//SpawnPlayerCamera(MPPC);
			MPPC->SetClientsCamera();
	}
}

void AMPGameMode::BeginPlay()
{
	Super::BeginPlay();

	_MaxNumberOfPlayers = FCString::Atoi(*(UGameplayStatics::ParseOption(OptionsString, "_MaxNumberOfPlayers")));
}

void AMPGameMode::RestartPlayer(AController* Player)
{
	Super::RestartPlayer(Player);
	UE_LOG(LogAMPGameMode, Warning, TEXT("Restarting Player!"));
	AMPPlayerController* PC = Cast<AMPPlayerController>(Player);

	if (PC->GetPawn())
	{
		TArray<AActor*> SpawnPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);
		
		TArray<AActor*> OtherPlayers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMPCharacter::StaticClass(), OtherPlayers);

		if (SpawnPoints.Num() > 0)
		{
			FVector FinalSpawnPoint = SpawnPoints[0]->GetActorLocation();
			float FurthestDist = 0;

			for (AActor* SP : SpawnPoints)
			{
				for (AActor* OP : OtherPlayers)
				{
					float Dist = FVector::Dist(SP->GetActorLocation(), OP->GetActorLocation());

					if(Dist > FurthestDist)
					{ 
						FurthestDist = Dist;
						FinalSpawnPoint = SP->GetActorLocation();
					}
				}
			}

			PC->GetPawn()->SetActorLocation(FinalSpawnPoint);
		}
		else
		{
			UE_LOG(LogAMPGameMode, Error, TEXT("MPGameMode_RestartPlayer: Cant find any spawn points!"))
		}
	}

	if (PC != NULL)
	{
		PC->SetClientsCamera();
		//UE_LOG(LogAMPGameMode, Warning, TEXT("Sent Request for clients to change their camera to the GameCamera!"));
		if (AMPCharacter* MPPC = Cast<AMPCharacter>(PC->GetPawn()))
		{
			MPPC->RequestItems_Client();
		}
	}

}

void AMPGameMode::PlayerAttackedPlayer(AActor* AttackerActor, AController* AttackerC, 
	AActor* DamagedActor, AController* DamagedC, FHitInfo HitInfo)
{
	UE_LOG(LogAMPGameMode, Warning, TEXT("Player Damaged a Player"));
	

	if (AMPCharacter* MPC = Cast<AMPCharacter>(DamagedActor))
	{
		if (AMPPlayerState* PS = Cast<AMPPlayerState>(MPC->GetPlayerState()))
		{
			if (AMPCharacter* AttackerChar = Cast<AMPCharacter>(AttackerActor))
			{
				if (!BlockedShot(AttackerChar, MPC))
				{
					MPC->RemoveSomeHP(50, DamagedC->PlayerState->GetPlayerName());

					if (MPC->GetHealth() <= 0)
					{
						PS->IncreaseDeathCounter();


						if (AMPPlayerState* AttackerPS = Cast<AMPPlayerState>(AttackerChar->GetPlayerState()))
						{
							AttackerPS->IncreaseKillCounter();
							RestartPlayer(DamagedC);
						}

					}
				}
			}
		}
	}
}

bool AMPGameMode::BlockedShot(AMPCharacter* Attacker, AMPCharacter* Defender)
{
	if (Defender->IsBlocking())
	{
		FVector DForward = Defender->GetMesh()->GetForwardVector();
		FVector OpponentDir = Defender->GetActorLocation() - Attacker->GetActorLocation();
		OpponentDir.Normalize();

		float DotProduct = FVector::DotProduct(DForward, OpponentDir);
		float Angle = acos(DotProduct);
		Angle = Angle * 57.2958;

		UE_LOG(LogAMPGameMode, Warning, TEXT("MPGameMode_BlockedShot: Block Angle = %f "), Angle);

		if (Angle < 90)
			return true;
	}

	return false;
}

void AMPGameMode::SpawnPlayerCamera(AController* OwningPlayer)
{
	if (OwningPlayer == NULL)
		return;

	AMPCamera* Cam = GetWorld()->SpawnActor<AMPCamera>();
	Cam->SetOwner(OwningPlayer);
	UE_LOG(LogAMPGameMode, Warning, TEXT("Spawned A Camera!"));
}

void AMPGameMode::Logout(AController* Exiting)
{
	if (APlayerController* ExitingPC = Cast<APlayerController>(Exiting))
	{
		bool bFoundPC = false;
		for (APlayerController* PC : _PlayerControllers)
		{
			if (PC == ExitingPC)
			{
				bFoundPC = true;
				break;
			}
		}

		if (!bFoundPC)
			return;

		_PlayerControllers.Remove(ExitingPC);
	}
}

void AMPGameMode::SendChatMessageToAllClients(const FChatMessage ChatMessage)
{
	/*for (APlayerController* PC : _PlayerControllers)
	{
		AMPPlayerController* MPPC = Cast<AMPPlayerController>(PC);
		if (MPPC != NULL)
		{
			MPPC->ClientReceiveNewChatMessage(ChatMessage);
		}
		else
		{
			UE_LOG(LogAMPGameMode, Error, TEXT("Cannot cast PC to MPPC"));
		}
	}*/
}
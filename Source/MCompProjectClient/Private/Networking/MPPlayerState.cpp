// Fill out your copyright notice in the Description page of Project Settings.

#include "Networking/MPPlayerState.h"
#include "Networking/MPGameState.h"
#include "Networking/MPPlayerController.h"
#include "UI/GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "MPGameInstance.h"
#include "UI/GameHUD.h"
#include "Engine/World.h"
#include "CoreGlobals.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogAMPPlayerState);

AMPPlayerState::AMPPlayerState()
{
	SetActorTickEnabled(true);
}

void AMPPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() != ROLE_Authority)
	{
		if (AGameHUD* HUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD()))
		{
			HUD->UpdateScoreboard();
		}
	}

	//if (GetLocalRole() != ROLE_Authority)
	//{
	//	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, TEXT("Client sent name to server"));
	//	UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//
	//	if (GI != NULL)
	//	{
	//		//GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow, TEXT("Client sent the server their display name"));
	//		Server_SetPlayerName(GI->GetPlayerName());
	//	}
	//
	//	//AskClientToSetName();
	//}
	//else
	//{
	//	AskClientToSetName();
	//}
	
}

void AMPPlayerState::CopyProperties(class APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

}

void AMPPlayerState::OverrideWith(class APlayerState* PlayerState)
{
	Super::OverrideWith(PlayerState);

}

void AMPPlayerState::GetNameAndKD(FString& PlayerName, int32& Kills, int32& Deaths)
{
	PlayerName = GetPlayerName();
	Kills = _Kills;
	Deaths = _Deaths;
}

bool AMPPlayerState::Server_SendChatMessage_Validate(const FChatMessage& Message)
{
	return true;
}

void AMPPlayerState::Server_SendChatMessage_Implementation(const FChatMessage& Message)
{
	UE_LOG(LogAMPPlayerState, Display, TEXT("(New Chat Message) %s: %s"), *Message._ClientsUsername, *Message._ChatMessage);
	Multicast_SendChatMessageToEveryone(Message);
}

bool AMPPlayerState::Multicast_SendChatMessageToEveryone_Validate(const FChatMessage& Message)
{
	return true;
}

void AMPPlayerState::Multicast_SendChatMessageToEveryone_Implementation(const FChatMessage& Message)
{
	if (AMPPlayerController* PC = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (AGameHUD* HUD = Cast<AGameHUD>(PC->GetHUD()))
		{
			HUD->AddNewChatMessage(Message._ClientsUsername, Message._ChatMessage);
		}
	}
}

void AMPPlayerState::IncreaseKillCounter()
{ 
	if (GetLocalRole() == ROLE_Authority)
	{
		_Kills++;
		//Multicast_GotAKill();

	}

}

void AMPPlayerState::Tick(float DeltaTime)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		if (_ClientsOldKillCount != _Kills || _ClientsOldUsername != GetPlayerName())
		{
			_ClientsOldKillCount = _Kills;
			_ClientsOldUsername = GetPlayerName();

			if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				if (AGameHUD* HUD = Cast<AGameHUD>(PC->GetHUD()))
				{
					HUD->UpdateScoreboard();
				}
			}
		}
	}
}

/*bool AMPPlayerState::Multicast_GotAKill_Validate()
{
	return true;
}

void AMPPlayerState::Multicast_GotAKill_Implementation()
{
	if (GetLocalRole() != ROLE_Authority)
	{
		if (AGameHUD* HUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD()))
		{
			HUD->UpdateScoreboard();
		}
	}
}*/

void AMPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPPlayerState, _Kills);
	DOREPLIFETIME(AMPPlayerState, _Deaths);
}




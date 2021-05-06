// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Networking/MPGameState.h"
#include "Net/UnrealNetwork.h"
#include "Networking/MPGameMode.h"
#include "Kismet/GameplayStatics.h"
//#include "EngineUtils.h"

AMPGameState::AMPGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if(GetLocalRole() == ROLE_Authority)
		PrimaryActorTick.bCanEverTick = true;
	else
		PrimaryActorTick.bCanEverTick = false;
}

void AMPGameState::Tick(float DeltaTime)
{
	if (!HasMatchStarted())
		return;

	if (GetLocalRole() == ROLE_Authority)
	{
		_TimerRemaining -= DeltaTime;

		if (_TimerRemaining <= 0)
		{	
			if (AMPGameMode* GameMode = Cast<AMPGameMode>(UGameplayStatics::GetGameMode(GetWorld())))
			{
				GameMode->EndMatch();
				
			}
		}
	}
}

void AMPGameState::Restart()
{
	_TimerRemaining = 20.0f;
}

FString AMPGameState::GetTimeRemaining()
{ 
	return FGameTimer::ConvertTimeForUI_Static(_TimerRemaining); 
}

void AMPGameState::AddScore(uint8 PlayerNumber)
{
	switch (PlayerNumber)
	{
	case 1:
		_PlayerOneScore++;
		break;
	case 2:
		_PlayerTwoScore++;
		break;
	default:
		break;
	}
}

void AMPGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPGameState, _PlayerOneScore);
	DOREPLIFETIME(AMPGameState, _PlayerTwoScore);
	DOREPLIFETIME(AMPGameState, _SomeNumber);
	DOREPLIFETIME(AMPGameState, _TimerRemaining);
}

void AMPGameState::IncreaseVariable()
{
	_SomeNumber++;
}

/*void AMPGameState::StoreNewChatMessage(const FChatMessage Message)
{
	_ClientChatMessages.Add(Message);

	if (_ClientChatMessages.Num() > _MaxStoredChatMessages)
	{
		_ClientChatMessages.RemoveAt(0);
	}
}


bool AMPGameState::SendChatMessageToServer_Validate(const FChatMessage Message)
{
	return true;
}

void AMPGameState::SendChatMessageToServer_Implementation(const FChatMessage Message)
{
	UE_LOG(LogTemp, Display, TEXT(Message._PlayerWhoSentMessage + ": " + Message._Message));
	StoreNewChatMessage(Message);

	UE_LOG(LogTemp, Display, TEXT("Sending New Chat Message To All Clients"));
	
	for (APlayerState* PS : PlayerArray)
	{
		PS->GetOwner();
		
		if (PS != NULL)
		{
			AMPPlayerController* PC = Cast<AMPPlayerController>(PS);

			if (PC != NULL)
				PC->SendClientNewChatMessage(Message);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not send message to clients!"));
			return;
		}
	}
}*/

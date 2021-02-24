// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/MPGameState.h"
//#include "GameFramework/PlayerState.h"
//#include "EngineUtils.h"

AMPGameState::AMPGameState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
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
}

void AMPGameState::IncreaseVariable()
{
	_SomeNumber++;
}

void AMPGameState::StoreNewChatMessage(const FChatMessage& Message)
{
	_ClientChatMessages.Add(Message);

	if (_ClientChatMessages.Num() > _MaxStoredChatMessages)
	{
		_ClientChatMessages.RemoveAt(0);
	}
}


bool AMPGameState::SendChatMessageToServer_Validate(const FChatMessage& Message)
{
	return true;
}

void AMPGameState::SendChatMessageToServer_Implementation(const FChatMessage& Message)
{
	SendClientNewChatMessage(Message);
}

bool AMPGameState::SendClientNewChatMessage_Validate(const FChatMessage& Message)
{
	return true;
}

void AMPGameState::SendClientNewChatMessage_Implementation(const FChatMessage& Message)
{
	StoreNewChatMessage(Message);
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Networking/MPPlayerState.h"
#include "Networking/MPGameState.h"
#include "Kismet/GameplayStatics.h"
#include "MPGameInstance.h"
#include "Engine/World.h"
#include "CoreGlobals.h"
#include "Net/UnrealNetwork.h"

AMPPlayerState::AMPPlayerState()
{

}

void AMPPlayerState::BeginPlay()
{
	Super::BeginPlay();

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

void AMPPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPPlayerState, _Kills);
	DOREPLIFETIME(AMPPlayerState, _Deaths);
}




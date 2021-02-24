// Fill out your copyright notice in the Description page of Project Settings.


#include "MPGameInstance.h"
#include "Networking/MPPlayerState.h"
#include "Networking/MPPlayerController.h"
#include "Networking/MPGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

UMPGameInstance::UMPGameInstance()
{

}

void UMPGameInstance::SetPlayerNameServer()
{
	AMPPlayerController* ThisController = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (ThisController != NULL)
	{
		AMPPlayerState* ThisPlayerState = Cast<AMPPlayerState>(ThisController->PlayerState);

		if (ThisPlayerState != NULL)
		{
			ThisPlayerState->SetPlayerName(_PlayerName);
		}
	}
}
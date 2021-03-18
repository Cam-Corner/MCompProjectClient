// Fill out your copyright notice in the Description page of Project Settings.

#include "MPGameInstance.h"
#include "Networking/MPPlayerController.h"
#include "Networking/MPPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "ItemData.h"
#include "UObject/ConstructorHelpers.h"

UMPGameInstance::UMPGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UItemData>
		ItemData(TEXT("ItemData'/Game/ItemData/ArmourItems.ArmourItems'"));
	
	if (ItemData.Succeeded())
	{
		_Items = ItemData.Object;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("MPGameInstance: Could not load ItemData!"));
	}
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

/*void UMPGameInstance::StoreNewChatMessage(const FChatMessage Message)
{
	_ClientChatMessages.Add(Message);

	if (_ClientChatMessages.Num() > _MaxStoredChatMessages)
	{
		_ClientChatMessages.RemoveAt(0);
	}
}*/
// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/MPPlayerController.h"
#include "Networking/MPPlayerState.h"
#include "Networking/MPGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "MPGameInstance.h"
#include "GameCamera.h"
#include "Engine/World.h"


AMPPlayerController::AMPPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void AMPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() != ROLE_Authority) {}
		//Server_IncreaseVariable();

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	SetInputMode(FInputModeGameOnly());
}

void AMPPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Server possessed a pawn! Changing clients camera!"));
		SetClientsCamera();
	}
}

bool AMPPlayerController::Server_SetPlayerName_Validate(const FString& Name)
{
	return true;
}

void AMPPlayerController::Server_SetPlayerName_Implementation(const FString& Name)
{

	AMPPlayerState* MPPlayerState = Cast<AMPPlayerState>(PlayerState);
	
	if (MPPlayerState != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Changed their name!, PlayerID: %i, OldName: %s, NewName %s"),
									MPPlayerState->GetUniqueID(), *MPPlayerState->GetPlayerName(), *Name);

		MPPlayerState->SetPlayerName(Name);
	}	
}

bool AMPPlayerController::AskClientToSetName_Validate()
{
	return true;
}

void AMPPlayerController::AskClientToSetName_Implementation()
{
	UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (GI != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow, TEXT("Client sent the server their display name"));
		Server_SetPlayerName(GI->GetPlayerName());
	}

}

//void AMPPlayerController::SendChatMessage_BP(const FString& ChatMessage)
//{
//	FChatMessage CM;
//	FString Username = PlayerState->GetPlayerName();
//	CM.SetMessage(Username, ChatMessage);
//	SendChatMessage(CM);
//}
//
//bool AMPPlayerController::SendChatMessage_Validate(const FChatMessage ChatMessage)
//{
//	return true;
//}
//
//void AMPPlayerController::SendChatMessage_Implementation(const FChatMessage ChatMessage)
//{
//	AMPGameMode* GM = Cast<AMPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
//
//	if (GM != NULL)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Client Sent a Message"));
//		GM->SendChatMessageToAllClients(ChatMessage);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Error, TEXT("Cannot access GameMode"));
//	}
//}
//
//bool AMPPlayerController::ClientReceiveNewChatMessage_Validate(const FChatMessage ChatMessage)
//{
//	return true;
//}
//
//void AMPPlayerController::ClientReceiveNewChatMessage_Implementation(const FChatMessage ChatMessage)
//{
//	_LastReceivedMessage = ChatMessage;
//}

bool AMPPlayerController::SetClientsCamera_Validate()
{
	return true;
}

void AMPPlayerController::SetClientsCamera_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Changed my camera - Called From Server!"));
	SetCameraToGameCamera();
}

void AMPPlayerController::SetCameraToGameCamera()
{
	TArray<AActor*> GameCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameCamera::StaticClass(), GameCameras);
	bool bSetCamera = false;

	for (AActor* AGC : GameCameras)
	{
		SetViewTarget(AGC);
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, TEXT("Changed camera to GameCamera"));
		bSetCamera = true;
		break;
	}

	if (!bSetCamera)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, TEXT("Can't find Game Camera"));

		UE_LOG(LogTemp, Error, TEXT("Could not find game camera!"));

		if (GameCameras.Num() == 0)
			UE_LOG(LogTemp, Error, TEXT("Game Camera TArray is NULL!"));
	}
}
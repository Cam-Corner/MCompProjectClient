// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/MPPlayerController.h"
#include "Networking/MPGameState.h"
#include "Networking/MPPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "MPGameInstance.h"
#include "Engine/World.h"


AMPPlayerController::AMPPlayerController()
{
}

void AMPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() != ROLE_Authority) {}
		//Server_IncreaseVariable();
}

void AMPPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (GetLocalRole() != ROLE_Authority)
	//{
	//	AMPPlayerState* MPPlayerState = Cast<AMPPlayerState>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerState);
	//
	//	if (MPPlayerState != NULL)
	//	{
	//		UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//
	//		if (GI != NULL)
	//		{
	//			MPPlayerState->Server_SetPlayerName(GI->GetPlayerName());
	//		}
	//	}
	//}
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

	//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, TEXT("Client was asked for their display name"));

	UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (GI != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow, TEXT("Client sent the server their display name"));
		Server_SetPlayerName(GI->GetPlayerName());
	}

}

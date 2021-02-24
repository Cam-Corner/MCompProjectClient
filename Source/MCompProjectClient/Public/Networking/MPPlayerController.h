// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "MPPlayerController.generated.h"


/**
 * 
 */



UCLASS()
class MCOMPPROJECTCLIENT_API AMPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMPPlayerController();

	UFUNCTION(Server, UnReliable, WithValidation)
		void Server_SetPlayerName(const FString& Name);


	UFUNCTION(Client, UnReliable, WithValidation)
		void AskClientToSetName();


	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:

	/** Holds server chat messages */
	//TArray<FChatMessage> _ChatMessages;
};

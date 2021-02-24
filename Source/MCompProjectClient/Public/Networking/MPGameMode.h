// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Net/UnrealNetwork.h"
#include "MPGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API AMPGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AMPGameMode();

public:
	virtual bool ReadyToStartMatch_Implementation() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

private:
	/** The Max number of players that are allowed in the server */
	int32 _MaxNumberOfPlayers;

	/** List of all the connected player controllers */
	TArray<class APlayerController*> _PlayerControllers;

	int32 _PlayerNum{ 1 };

};

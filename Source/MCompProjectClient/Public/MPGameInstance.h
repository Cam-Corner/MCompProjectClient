// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API UMPGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMPGameInstance();

	/** Set the name of the player, this name will be sent to the server when it connects  */
	UFUNCTION(BlueprintCallable)
		void SetPlayerName(const FString& Name) { _PlayerName = Name; }

	/** Gets the player name that was set  */
	UFUNCTION(BlueprintCallable)
		const FString GetPlayerName() { return _PlayerName; }

	/** Update the player name on the server */
	UFUNCTION(BlueprintCallable)
		void SetPlayerNameServer();

private:
	FString _PlayerName{ "No Name Given" };

};

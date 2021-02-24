// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MPPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API AMPPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMPPlayerState();

	//used to copy properties from the current playerState to the passed one
	virtual void CopyProperties(class APlayerState* PlayerState) override;

	//used to override the current playerstate with the properties of the passed one
	virtual void OverrideWith(class APlayerState* PlayerState) override;

	/** Sets the player name in blueprints*/
	UFUNCTION(BlueprintCallable)
		void SetPlayerName_BPCallable(const FString& Name) { SetPlayerName(Name); }

	/** Gets the player name in blueprints*/
	UFUNCTION(BlueprintCallable)
		const FString GetPlayerName_BPCallable() { return GetPlayerName(); }

	virtual void BeginPlay() override;
};

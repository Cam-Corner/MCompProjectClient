// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MPPlayerState.generated.h"

USTRUCT(BlueprintType)
struct FChatMessage
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString _ClientsUsername = "Unknown";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString _ChatMessage = "...";

public:
	void SetMessage(const FString& Username, const FString& ChatMessage)
	{
		_ClientsUsername = Username;
		_ChatMessage = ChatMessage;
	}
};

DECLARE_LOG_CATEGORY_EXTERN(LogAMPPlayerState, Log, All);

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

	/** Increase the kill counter
	* @ Will only work when being called on the server
	*/
	void IncreaseKillCounter() { if (GetLocalRole() == ROLE_Authority) _Kills++; }

	/** Increase the Death counter
	* @ Will only work when being called on the server
	*/
	void IncreaseDeathCounter() { if(GetLocalRole() == ROLE_Authority) _Deaths++; }

	/** Get the KD and name of this player
	* @ The passed in variable should be used to save the KD
	*/
	UFUNCTION(BlueprintCallable)
	void GetNameAndKD(FString& PlayerName, int32& Kills, int32& Deaths);

	/*
	* Send a chat message to the server to be rpc to all clients
	*/
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendChatMessage(const FChatMessage& Message);

	/*
	* Send the message to all clients
	*/
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_SendChatMessageToEveryone(const FChatMessage& Message);

protected:
	UPROPERTY(Replicated)
		int32 _Kills { 0 };

	UPROPERTY(Replicated)
		int32 _Deaths { 0 };

};

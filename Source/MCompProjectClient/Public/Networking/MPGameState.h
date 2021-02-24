// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Networking/MPPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "MPGameState.generated.h"


/**
 * 
 */
USTRUCT(Blueprintable)
struct FChatMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString _PlayerWhoSentMessage{ "Unknown" };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString _Message{ "Nothing" };
};

UCLASS()
class MCOMPPROJECTCLIENT_API AMPGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMPGameState(const FObjectInitializer& ObjectInitializer);

	void AddScore(uint8 PlayerNumber);


	UFUNCTION(Server, UnReliable, WithValidation, BlueprintCallable)
		void SendChatMessageToServer(const FChatMessage& Message);

	UFUNCTION(Client, UnReliable, WithValidation)
		void SendClientNewChatMessage(const FChatMessage& Message);

	UFUNCTION(BlueprintCallable)
		TArray<FChatMessage> GetChatMessages() { return _ClientChatMessages; }

public:

	UPROPERTY(Replicated)
		int32 _PlayerOneScore;

	UPROPERTY(Replicated)
		int32 _PlayerTwoScore;

	UPROPERTY(Replicated)
		int32 _SomeNumber;

	void IncreaseVariable();

	/** Store New Client Message */
	void StoreNewChatMessage(const FChatMessage& Message);

private:
	/** Stores all the messages the clients have sent to each other*/
	TArray<FChatMessage> _ClientChatMessages;

	int32 _MaxStoredChatMessages = 15;
};

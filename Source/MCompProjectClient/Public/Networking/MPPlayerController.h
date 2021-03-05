// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPPlayerController.generated.h"

/**
 * 
 */

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

	UFUNCTION(Client, Unreliable, WithValidation)
		void SetClientsCamera();

	//UFUNCTION(Server, Reliable, WithValidation)
	//	void SendChatMessage(const FChatMessage ChatMessage);
	//
	//UFUNCTION(Client, Reliable, WithValidation)
	//	void ClientReceiveNewChatMessage(const FChatMessage ChatMessage);
	//
	//UFUNCTION(BlueprintCallable)
	//	void SendChatMessage_BP(const FString& ChatMessage);
	//
	//UFUNCTION(BlueprintCallable)
	//	FChatMessage GetLastChatMessage_BP() { return _LastReceivedMessage; }

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;
	
	void SetCameraToGameCamera();;

private:
	FChatMessage _LastReceivedMessage;
};

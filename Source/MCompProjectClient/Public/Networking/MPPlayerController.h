// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MPPlayerController.generated.h"

/**
 * 
 */
DECLARE_LOG_CATEGORY_EXTERN(LogAMPPlayerController, Log, All);


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

	UFUNCTION(Client, Reliable)
		void Client_GameStarted(bool bStarted);

	UFUNCTION(Server, Reliable)
		void Server_UpdateItems();

	UFUNCTION(Client, Reliable)
		void Client_KickFromServer(const FString& KickReason);

	UFUNCTION(Client, Reliable)
		void Client_GameEnded();

	UFUNCTION(Client, Reliable)
		void Client_TravelingToNewMap(bool bTraveling);

	UFUNCTION(Server, Reliable)
		void Server_SendMapVote(int8 MapVote);


	//UFUNCTION(Client, Reliable)
	//	void Client_SetHasControl(bool bHasControl);

	//UFUNCTION(Server, Reliable, WithValidation)
	//	void SendChatMessage(const FChatMessage& ChatMessage);
	//
	//UFUNCTION(Client, Reliable, WithValidation)
	//	void ClientReceiveNewChatMessage(const FChatMessage& ChatMessage);
	//
	//UFUNCTION(BlueprintCallable)
	//	void SendChatMessage_BP(const FString& ChatMessage);
	//
	//UFUNCTION(BlueprintCallable)
	//	FChatMessage GetLastChatMessage_BP() { return _LastReceivedMessage; }

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void OnPossess(APawn* InPawn) override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	void SetCameraToGameCamera();

private:
	/*
	* Controls the YawMovement of the controller
	*/
	UFUNCTION()
		void YawInput(float Value);

	/*
	* Controls the pitch of the player controller
	*/
	UFUNCTION()
		void PitchInput(float Value);

	float _CameraSensitivity{ 12.0f };

	//FChatMessage _LastReceivedMessage;

	/** Is the player allowed to control this pawn at this current moment */
	bool _bHasControl{ true };

	/*
	* Cached CameraActor that this PC Uses
	*/
	class AMPCamera* _CameraActor;

	/*
	* Is the player currently typing
	*/
	bool _bTyping = false;

	UFUNCTION()
		void ToggleChat();

	/*
	* Cached AGameHUD
	* @can be null
	*/
	class AGameHUD* _GameHUD;

	int8 _CurrentMapVote{ 0 };

protected:
	UFUNCTION()
	void EnableScoreboard();
	
	UFUNCTION()
	void DisableScoreboard();


};

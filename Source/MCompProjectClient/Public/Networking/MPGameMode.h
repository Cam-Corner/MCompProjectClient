// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MPGameMode.generated.h"

UENUM()
enum EWeaponType
{
	EWT_NotSet UMETA("Not Set"),
	EWT_Sword UMETA("Sword"),
};

USTRUCT()
struct FHitInfo
{
	GENERATED_BODY()
	/** The name of the player that was attacking */
	FName _PlayerAttacker = "NotSet";
	
	/** The name of that got hit */
	FName _PlayerHit = "NotSet";

	/** The direction the hit came from */
	FVector _HitDirection{ 0, 0, 0 };

	/** The weapon type that caused the player to be hit */
	TEnumAsByte<EWeaponType> _WeaponType;
};

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

	virtual void RestartPlayer(AController* Player) override;

	virtual void HandleMatchHasStarted() override;

	/*
	* This function is used to spawn a player and then possess it using the provided PlayerController
	*/
	//void SpawnPlayer(class AMPPlayerController* MPPC);

	//void SendChatMessageToAllClients(const struct FChatMessage ChatMessage);

	/** Call to calculate damage between player attacks */
	void PlayerAttackedPlayer(AActor* AttackerActor, AController* AttackerC, 
		AActor* DamagedActor, AController* DamagedC, FHitInfo HitInfo);

protected:
	bool BlockedShot(class AMPCharacter* Attacker, class AMPCharacter* Defender);

private:
	/** The Max number of players that are allowed in the server */
	int32 _MaxNumberOfPlayers;

	/** List of all the connected player controllers */
	TArray<class APlayerController*> _PlayerControllers;

	int32 _PlayerNum{ 1 };

	/*
	* The pawn that will be used when spawning a character
	* Not set by the defualt gamemode beause I want control over when I spawn it
	*/
	//class AMPCharacter* _DefualtCharacter;

	/** Has the game started */
	bool _bGameCanStart = false;

	/*
	* A List of all the player camera's in the scene
	* Useful the server has access to the camera incase the server wants to do something with it
	*/

};

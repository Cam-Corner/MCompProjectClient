// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MPGameMode.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAMPGameMode, Log, All);

#define LEVEL_FORREST FString("/Game/Levels/lvl_Level")
#define LEVEL_EXAMPLEMAP FString("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap")

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

	virtual void EndMatch() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	virtual void RestartPlayer(AController* Player) override;

	virtual void HandleMatchHasStarted() override;

	virtual void Logout(AController* Exiting) override;

	/*
	* 0 = no map vote
	* 1 = Forrest Level
	*/
	void ChangeMapVote(int8 CurrentMapVote, int8 NewMapVote);

	void LoadLevel(const FString LevelToLoad);
	/*
	* This function is used to spawn a player and then possess it using the provided PlayerController
	*/
	//void SpawnPlayer(class AMPPlayerController* MPPC);

	void SendChatMessageToAllClients(const struct FChatMessage ChatMessage);

	/** Call to calculate damage between player attacks */
	void PlayerAttackedPlayer(AActor* AttackerActor, AController* AttackerC, 
		AActor* DamagedActor, AController* DamagedC, FHitInfo HitInfo);

protected:
	bool BlockedShot(class AMPCharacter* Attacker, class AMPCharacter* Defender);

	void SpawnPlayerCamera(AController* OwningPlayer);
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
	TArray<class AMPCamera*> _PlayerCameras;

	/*
	* The current map votes
	* The array index reprisents an map and the value is the vote amount
	* Example: 
	* index 0 = amount of people not voted
	* index 1 = Map 'X' and the index value = 2 (which means 2 people voted for it)
	*/
	TArray<int8> _MapVotes;

	bool _bVotingForMap{ false };

	float _VotingTimer{ 10.0f };
};

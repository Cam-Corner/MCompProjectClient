// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Networking/CharacterGearHelpers.h"
#include "Utility/PIDController.h"
#include "MPCharacter.generated.h"


//class PIDController; 
DECLARE_LOG_CATEGORY_EXTERN(LogAMPCharacter, Log, All);

UCLASS()
class MCOMPPROJECTCLIENT_API AMPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMPCharacter();

	float _TempTimer = 0.25f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Restart() override;

	/** Edit the players health */
	void RemoveSomeHP(float Amount, FString Name) { 
		_Health -= Amount; 
		UE_LOG(LogTemp, Warning, TEXT("%s Took Damage! HP = %f"), *Name, _Health);
	}

	/** Get the players HP */
	UFUNCTION(BlueprintCallable)
	float GetHealth() { return _Health; }

	/*
	*Is the player blocking
	*/
	bool IsBlocking() { return _bBlocking; }

	/** Ask client to send their equiped items */
	UFUNCTION(Client, Reliable, WithValidation)
		void RequestItems_Client();

/*=======
Server Functions
=====*/
protected:
	/** Sets the unit Vector on the server */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetUnitVector(FVector UnitV);

	/** Sets the unit Vector on the server */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetLookDirection(FVector2D LookDirection);

	UFUNCTION(Server, Reliable)
		void Server_SetRotation(FRotator Rotation);

	/** Sets the blocking state on the server */
	UFUNCTION(Server, UnReliable, WithValidation)
		void Server_SetEnabledBlock(bool bBlocked);

	/** Sets the attacking state on the server */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetAttacking(bool bAttacking);

	/** Tells all the clients that this actor has stopped attacking */
	UFUNCTION(NetMulticast, reliable, WithValidation)
		void Multicast_StopAttacking();

	/** Called on the server (should be from the PC) to be excuted on everything */
	UFUNCTION(NetMulticast, reliable, WithValidation)
		void SetupMeshes_Multicast(uint8 HelmetID, uint8 HairID, uint8 FaceID,
			uint8 ShouldersID, uint8 BodyID, uint8 GlovesID, uint8 BeltID, uint8 ShoesID
		/*struct FFinalCharacterGear EquipedGear*/);
	
	/** Send the items equiped to the server */
	UFUNCTION(Server, Reliable, WithValidation)
		void SendItemsEquiped_Server(uint8 HelmetID, uint8 HairID, uint8 FaceID,
			uint8 ShouldersID, uint8 BodyID, uint8 GlovesID, uint8 BeltID, uint8 ShoesID
		/*struct FFinalCharacterGear EquipedGear*/);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_EquipWeaponProperly();

	UFUNCTION(NetMulticast, reliable, WithValidation)
		void Multicast_EquipNewWeapon(class AWeaponBase* Weapon);

	/** Equip a weapon for this actor and call on all clients */
	UFUNCTION(NetMulticast, reliable, WithValidation)
		void EquipWeapon_Multicast();

	/** Tell the server im ready to recieve intitial stuff */
	//UFUNCTION(Server, Reliable)
	//	void Server_ReadyForIntitialSetup();

/*====
Components	
====*/
protected:	
	/** reference to the animation instance */
	class UMPCharacterAnimInstance* _AnimInstance;

	/** Static mesh that is used for right hand items*/
	//UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* _RightHandItem;
	
	/** Static mesh for holding left handed items*/
	class UStaticMeshComponent* _LeftHandItem;

	///** Box Collider used to check what actors are inside when attacking
	//	@Currently temporary and should be removed and improved later
	//	@Should only be useful on the server*/
	//UPROPERTY(EditAnywhere)
	//class UBoxComponent* _SwordHitBox;

	//class AWeaponBase* _Weapon;
/*====
Functions
====*/
protected:
	/** Get up and down movement input */
	UFUNCTION()
		void MoveUp(float Value);

	/** Gets left and right movement input */
	UFUNCTION()
		void MoveRight(float Value);

	/** Get up and down Look Direction input */
	UFUNCTION()
		void LookUp(float Value);

	/** Gets left and right Look Direction input */
	UFUNCTION()
		void LookRight(float Value);

	UFUNCTION()
		void StartedBlocking();

	UFUNCTION()
		void StoppedBlocking();

	UFUNCTION()
		void StartAttacking();

	/* works out the final movement */
	void FinalMovement(float DeltaTime);

	/** Work out if anyone is being hit */
	void CheckForHits();

	/** function that is called when the sword overlaps something */
	UFUNCTION()
		void SwordHitBoxOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& HitResult);

	/** function that is called when the sword leaves an overlaped object */
	UFUNCTION()
		void SwordHitBoxOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*
	* Setup the Skeletal Character Meshes when asked 
	* Uses the _EquipedGear set by the server to setup the right SK Meshes
	*/
	void SetupSkeletalCharacterMeshes(uint8 HelmetID, uint8 HairID, uint8 FaceID, 
		uint8 ShouldersID, uint8 BodyID, uint8 GlovesID, uint8 BeltID, uint8 ShoesID 
		/*struct FFinalCharacterGear EquipedGear*/);

	/** Quit the game */
	UFUNCTION()
		void QuitGame();

	/*
	* smooths the rotation to match where the play is currently facing
	*/
	void SmoothRotationToDirection(float DeltaTime);
/*====
Variables
====*/
protected:
	/** Characters Health */
	UPROPERTY(Replicated)
	float _Health{ 100 };

	/** Characters Stamina */
	float _Stamina{ 100 };

	/** Is the character currently blocking */
	UPROPERTY(Replicated)
	bool _bBlocking{ false };

	/** Is the character currently attacking */
	UPROPERTY(Replicated)
		bool _bIsAttacking{ false };

	float _AttackDuration{ 0 };

	UPROPERTY(Replicated)
	struct FFinalCharacterGear _EquipedGear;

	/** The final movement vector of this frame */
	UPROPERTY(Replicated)
	FVector _FinalMovementDirection{ 0, 0, 0 };

	/** The direction the other thumbstick is facing */
	UPROPERTY(Replicated)
	FVector2D _LookDirection{ 0, 0 };

	/** Last direction that they moved */
	FVector _LastMovementDirection{ 0, 0, 0 };

	/** Last direction that they looked */
	FVector _LastLookDirection2D{ 0, 0, 0 };
	/*
	* The actors that are in the swords collision box
	* Only Useful for the server
	* Wont be used for clients
	*/
	TArray<AMPCharacter*> _PlayersInSwordCol;

	/*
	* The actors that have already been hit by the sword 
	* Gets cleared after every sword swing
	* used so players don't get hit twice by the same actor
	* only useful for the server
	* won't be used on the client
	*/
	TArray<AMPCharacter*> _PlayersHitBySword;

	UPROPERTY(EditAnywhere)
	class USkeletalMesh* _ItemMesh;

	class USkeletalMeshComponent* _HelmetSM;
	class USkeletalMeshComponent* _HairSM;
	class USkeletalMeshComponent* _FaceSM;
	class USkeletalMeshComponent* _ShouldersSM;
	class USkeletalMeshComponent* _GlovesSM;
	class USkeletalMeshComponent* _BeltSM;
	class USkeletalMeshComponent* _ShoesSM;

	PIDController _RotationPID{ 0.2f, 0.0f, 0.01f };

	float CurrentYaw = 1;

public:
	/* Called when the actor needs to pickup a weapon */
	virtual void PickupWeapon(class AWeaponBase* Weapon);

	/* Called when the actor needs to drop a weapon that they are currently holding */
	virtual void DropWeapons();

	/* The Current Weapon that they are holding
	* @Sword and shield are hidden because they are just hidden while using a special weapon
	*/
	class AWeaponBase* _SpecialWeaponEquiped;
};

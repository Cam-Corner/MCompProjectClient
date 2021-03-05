// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MPCharacter.generated.h"

UCLASS()
class MCOMPPROJECTCLIENT_API AMPCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMPCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Edit the players health */
	void RemoveSomeHP(float Amount, FString Name) { 
		_Health -= Amount; 
		UE_LOG(LogTemp, Warning, TEXT("%s Took Damage! HP = %f"), *Name, _Health);
	}

	/** Get the players HP */
	UFUNCTION(BlueprintCallable)
	float GetHealth() { return _Health; }

/*=======
Server Functions
=====*/
protected:
	/** Sets the unit Vector on the server */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetUnitVector(FVector UnitV);

	/** Sets the blocking state on the server */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetEnabledBlock(bool bBlocked);

	/** Sets the attacking state on the server */
	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SetAttacking(bool bAttacking);

	/** Tells all the clients that this actor has stopped attacking */
	UFUNCTION(NetMulticast, Reliable, WithValidation)
		void Multicast_StopAttacking();

/*====
Components	
====*/
protected:	
	/** reference to the animation instance */
	class UMPCharacterAnimInstance* _AnimInstance;

	/** Static mesh that is used for right hand items*/
	//UPROPERTY(EditAnywhere)
	//class UStaticMeshComponent* _RightHandItem;
	
	/** Static mesh for holding left handed items*/
	class UStaticMeshComponent* _LeftHandItem;

	///** Box Collider used to check what actors are inside when attacking
	//	@Currently temporary and should be removed and improved later
	//	@Should only be useful on the server*/
	//UPROPERTY(EditAnywhere)
	//class UBoxComponent* _SwordHitBox;

	class AWeaponBase* _Weapon;
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

	UFUNCTION()
		void StartedBlocking();

	UFUNCTION()
		void StoppedBlocking();

	UFUNCTION()
		void StartAttacking();

	/* works out the final movement */
	void FinalMovement();

	/** Work out if anyone is being hit */
	void CheckForHits();

	/** function that is called when the hand overlaps something */
	UFUNCTION()
		void SwordHitBoxOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& HitResult);

	/** function that is called when the hand leaves an overlaped object */
	UFUNCTION()
		void SwordHitBoxOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


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

	/** The final movement vector of this frame */
	UPROPERTY(Replicated)
	FVector _FinalMovementDirection{ 0, 0, 0 };

	/** The actors that are in the swords collision box
		@Only Useful for the server
		@Wont be used for clients
	*/
	//TArray<AMPCharacter*> _PlayersInSwordCol;

	/** The actors that have already been hit by the sword 
		@Gets cleared after every sword swing
		@used so players don't get hit twice by the same actor
		@only useful for the server
		@won't be used on the client
	*/
	TArray<AMPCharacter*> _PlayersHitBySword;
};

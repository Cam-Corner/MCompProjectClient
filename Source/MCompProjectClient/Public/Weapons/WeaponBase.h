// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAWeaponBase, Log, All);

UCLASS()
class MCOMPPROJECTCLIENT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void Attack();

	virtual void DropWeapon();

	virtual void PickupWeapon(class AController* NewOwner, class AMPCharacter* CharacterToAttachTo, const FName SocketName = NAME_None);
	///** function that is called when the hand overlaps something */
	//UFUNCTION()
	//	void SwordHitBoxOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	//		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	//		const FHitResult& HitResult);
	//
	///** function that is called when the hand leaves an overlaped object */
	//UFUNCTION()
	//	void SwordHitBoxOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	//		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//
	//
	//TArray<class AMPCharacter*>& GetCharacterArray() { return _PlayersInSwordCol; }

protected:
	class UStaticMeshComponent* _Weapon;
	//
	///** The actors that are in the swords collision box
	//@Only Useful for the server
	//@Wont be used for clients
	//
	//TArray<class AMPCharacter*> _PlayersInSwordCol;
	//
	///** The actors that have already been hit by the sword
	//	@Gets cleared after every sword swing
	//	@used so players don't get hit twice by the same actor
	//	@only useful for the server
	//	@won't be used on the client
	//*/
	//TArray<class AMPCharacter*> _PlayersHitBySword;


	/** The MPCharacter the weapon is attached to */
	class AMPCharacter* _CharacterAttachedTo;

	bool _bWeaponIsEquipedByPlayer = false;

	UPROPERTY(VisibleAnywhere, Category = "Pickup Collision")
	class USphereComponent* _WeaponPickupHitbox;

	UFUNCTION()
		void WeaponPickupOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& HitResult);

	/** function that is called when the sword leaves an overlaped object */
	UFUNCTION()
		void WeaponPickupOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

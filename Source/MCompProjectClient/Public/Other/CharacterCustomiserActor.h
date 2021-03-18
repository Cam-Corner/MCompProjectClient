// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterCustomiserActor.generated.h"

UCLASS()
class MCOMPPROJECTCLIENT_API ACharacterCustomiserActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterCustomiserActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/** Set this actors visibility */
	void SetVisibility(bool bVisible) { SetActorHiddenInGame(!bVisible); }

private:
	/** The skeletal Mesh that changes the helmet */
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* _HelmetSM;

	/** The skeletal Mesh that changes the hair */
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* _HairSM;

	/** The skeletal Mesh that changes the face */
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* _FaceSM;

	/** The skeletal Mesh that changes the Shoulders */
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* _ShouldersSM;

	/** The skeletal Mesh that changes the Body */
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* _BodySM;

	/** The skeletal Mesh that changes the Gloves */
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* _GlovesSM;

	/** The skeletal Mesh that changes the Belt */
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* _BeltSM;

	/** The skeletal Mesh that changes the Shoes */
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* _ShoesSM;

public:
	/** Increase the amour int value
	* @param 1 = Helmet
	* @param 2 = Hair
	* @param 3 = Face
	* @param 4 = Shoulders
	* @param 5 = Body
	* @param 6 = Gloves
	* @param 7 = Belt
	* @param 8 = Shoes
	*/
	void IncreaseArmourInt(uint8 Type);

	/** Decrease the amour int value
	* @param 1 = Helmet
	* @param 2 = Hair
	* @param 3 = Face
	* @param 4 = Shoulders
	* @param 5 = Body
	* @param 6 = Gloves
	* @param 7 = Belt
	* @param 8 = Shoes
	*/
	void DecreaseArmourInt(uint8 Type);

	uint8 _CurrentHeadID = 0;
	uint8 _CurrentHairID = 0;
	uint8 _CurrentFaceID = 0;
	uint8 _CurrentShouldersID = 0;
	uint8 _CurrentBodyID = 0;
	uint8 _CurrentGlovesID = 0;
	uint8 _CurrentBeltID = 0;
	uint8 _CurrentShoesID = 0;

private:
	uint8 NewCurrentGearID(const uint8& CurrentID, uint8 MaxID, bool bIncreasing);

	void UpdateBodyParts();
};

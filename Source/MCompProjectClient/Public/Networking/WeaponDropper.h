// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "WeaponDropper.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAWeaponDropper, Log, All);


UCLASS()
class MCOMPPROJECTCLIENT_API AWeaponDropper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponDropper();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	/* default timer between weapon drops */
	float _DefaultWeaponTimer = 15.0f;

	/* Current Weapon Timer */
	float _CurrentWeaponTimerDrop = 0;

	void DropAWeapon();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MultiCast_DropWeapon();

	/* Bow weapon base class */
	TSubclassOf<class ABowWeapon> _BowWeaponClass;
};

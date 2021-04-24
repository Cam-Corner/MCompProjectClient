// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/WeaponBase.h"
#include "BowWeapon.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogABowWeapon, Log, All);

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API ABowWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	ABowWeapon();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;


protected:

private:

	/* bow rate of fire
	* @The delay between each arrow shot before they can shoot again
	*/
	UPROPERTY(EditAnywhere, Category = "Weapon Stats")
	float _NextArrowDelayTime = 1.5f;

	/* Time left before the next arrow can be shot again */
	float _NextArrowTimer = 0;

	/* How many shots are left
	* also change the starting value to decide how many they start with
	*/
	UPROPERTY(Replicated)
	uint32 _ArrowsLeft = 5;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AArrow> _ArrowClass;

};

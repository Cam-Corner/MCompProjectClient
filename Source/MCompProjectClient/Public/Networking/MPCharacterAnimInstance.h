// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MPCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API UMPCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UMPCharacterAnimInstance();

	/** Set Current Speed */
	void SetCurrentSpeed(float Value) { _CurrentSpeed = Value; }

	/** set whether the playing is blocking */
	void SetIsBlocking(bool bBlocking) { _bBlocking = bBlocking; }

	void SetIsAttacking(bool bIsAttacking) { _bIsAttacking = bIsAttacking; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float _CurrentSpeed{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool _bBlocking{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool _bIsAttacking{ false };
};
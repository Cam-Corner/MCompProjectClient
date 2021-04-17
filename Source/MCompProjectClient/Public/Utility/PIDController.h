// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MCOMPPROJECTCLIENT_API PIDController
{
public:
	PIDController();
	PIDController(float P, float I, float D)
		: _Proportional(P), _Integral(I), _Derivative(D) {}

	~PIDController();

	/** returns a new value to smoothly go to the desired value */
	float Update(const float& CurrentValue, const float& DesiredValue, const float DeltaTime);

	/** set the PID Values */
	void SetPIDValues(float P, float I, float D) { _Proportional = P; _Integral = I; _Derivative = D; }

private:
	float _ErrorPrior{ 0 };
	float _IntegralPrior{ 0 };

	float _Proportional{ 0 };
	float _Integral{ 0 };
	float _Derivative{ 0 };
};

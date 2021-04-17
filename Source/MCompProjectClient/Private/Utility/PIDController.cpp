// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/PIDController.h"

PIDController::PIDController()
{
}

PIDController::~PIDController()
{
}

float PIDController::Update(const float& CurrentValue, const float& DesiredValue, const float DeltaTime)
{
	float Error = DesiredValue - CurrentValue;
	float Integral = _IntegralPrior + Error * DeltaTime;
	float Derivative = (Error - _ErrorPrior) / DeltaTime;
	float Output = (_Proportional * _ErrorPrior) + (_Integral * Integral) + (_Derivative * Derivative);

	_ErrorPrior = Error;
	_IntegralPrior = Integral;

	return Output;
}

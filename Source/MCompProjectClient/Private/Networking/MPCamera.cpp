// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/MPCamera.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMPCamera::AMPCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	_RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = _RootComp;

	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	_Camera->SetupAttachment(_RootComp);
}

// Called when the game starts or when spawned
void AMPCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMPCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMPCamera::YawInput(float Value)
{
	if (Value != 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(4, 0.5f, FColor::Blue, TEXT("YawControl"));

	}
}

void AMPCamera::PitchInput(float Value)
{
	if (Value != 0.0f)
	{
		GEngine->AddOnScreenDebugMessage(4, 0.5f, FColor::Blue, TEXT("YawControl"));
		
	}
}


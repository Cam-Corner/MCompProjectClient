// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCamera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGameCamera::AGameCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*====
	Setup the camera
	====*/
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Game Camera"));
	_Camera->SetupAttachment(RootComponent);
	//_Camera->SetFieldOfView(70);
}

// Called when the game starts or when spawned
void AGameCamera::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGameCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


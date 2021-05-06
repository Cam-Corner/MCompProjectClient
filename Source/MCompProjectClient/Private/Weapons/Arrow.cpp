// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Arrow.h"
#include "Net/UnrealNetwork.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	_ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Arrow Mesh"));
	RootComponent = _ArrowMesh;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if(GetLocalRole() == ROLE_Authority)
		AddActorWorldOffset(GetActorForwardVector() * (_ArrowSpeed * DeltaTime));
}

void AArrow::Mulitcast_UpdatePosition_Implementation(FVector StartPos, float CurrentDeltaTime)
{

}

void AArrow::Server_RequestPosUpdate_Implementation()
{

}



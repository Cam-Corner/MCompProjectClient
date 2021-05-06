// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/BowWeapon.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"
#include "Networking/MPCharacter.h"
#include "Weapons/Arrow.h"

DEFINE_LOG_CATEGORY(LogABowWeapon);

ABowWeapon::ABowWeapon()
{
	bReplicates = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ItemMesh(TEXT("StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Bow01SM.Bow01SM'"));

	_Weapon->SetGenerateOverlapEvents(false);
	_Weapon->AddLocalRotation(FRotator(0, 0, -90));
	_Weapon->SetStaticMesh(ItemMesh.Object);
	_Weapon->AddLocalOffset(FVector(-10, 3, 0));
	_Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_Weapon->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	_Weapon->SetCollisionResponseToAllChannels(ECR_Ignore);

	//find the bow weapon bp class
	static ConstructorHelpers::FClassFinder<AArrow> Arrow(TEXT("/Game/Weapons/bp_Arrow"));
	if (Arrow.Succeeded())
	{
		_ArrowClass = Arrow.Class;
	}
	else
	{
		UE_LOG(LogABowWeapon, Error, TEXT("Coud not find blueprint child class called bp_Arrow (Constructor Helpers)"));
	}
}

void ABowWeapon::BeginPlay()
{
	Super::BeginPlay();


}

void ABowWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority)
		return;

	if (_NextArrowTimer > 0)
		_NextArrowTimer -= DeltaTime;

	if (_ArrowsLeft <= 0)
	{
		if (_CharacterAttachedTo != NULL)
		{
			_CharacterAttachedTo->DropWeapons();
		}

		GetWorld()->DestroyActor(this);
		UE_LOG(LogAWeaponBase, Warning, TEXT("Destroyed Weapon"));
	}

}

void ABowWeapon::Attack()
{
	if (GetLocalRole() != ROLE_Authority)
		return;

	if (_NextArrowTimer > 0)
		return;

	if (_ArrowClass != NULL)
	{
		AActor* Arrow = GetWorld()->SpawnActor(_ArrowClass);
		Arrow->SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
		Arrow->SetActorLocation(GetActorLocation() + (GetActorForwardVector() * 5));
		Arrow->SetOwner(GetOwner());

	}

	_ArrowsLeft -= 1;
	_NextArrowTimer = _NextArrowDelayTime;

}

void ABowWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABowWeapon, _ArrowsLeft);

}

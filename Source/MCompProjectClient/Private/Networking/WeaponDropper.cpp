// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/WeaponDropper.h"
#include "UObject/ConstructorHelpers.h"
#include "Weapons/WeaponBase.h"
#include "Weapons/BowWeapon.h"

DEFINE_LOG_CATEGORY(LogAWeaponDropper);

// Sets default values
AWeaponDropper::AWeaponDropper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//find the bow weapon bp class
	static ConstructorHelpers::FClassFinder<ABowWeapon> BowWeapon(TEXT("/Game/Weapons/bp_BowWeapon"));
	if (BowWeapon.Succeeded())
	{
		_BowWeaponClass = BowWeapon.Class;
	}
	else
	{
		UE_LOG(LogAWeaponDropper, Error, TEXT("Coud not find blueprint child class called bp_BowWeapon (Constructor Helpers)"));
	}
}

// Called when the game starts or when spawned
void AWeaponDropper::BeginPlay()
{
	Super::BeginPlay();
	
	_CurrentWeaponTimerDrop = _DefaultWeaponTimer;
}

// Called every frame
void AWeaponDropper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority)
		return;

	if (_CurrentWeaponTimerDrop > 0)
	{
		_CurrentWeaponTimerDrop -= DeltaTime;
	}
	else
	{
		_CurrentWeaponTimerDrop = 9999;//_DefaultWeaponTimer;
		//MultiCast_DropWeapon();
		DropAWeapon();
	}

}

void AWeaponDropper::DropAWeapon()
{
	AWeaponBase* Weapon = Cast<AWeaponBase>(GetWorld()->SpawnActor(_BowWeaponClass));

	if (Weapon != NULL)
	{
		Weapon->SetActorLocation(GetActorLocation() + FVector(0, 0, 50));
		//Weapon->DropWeapon();
	}

	UE_LOG(LogAWeaponDropper, Display, TEXT("A New Weapon Has Been Dropped"));
}

bool AWeaponDropper::MultiCast_DropWeapon_Validate()
{
	return true;
}

void AWeaponDropper::MultiCast_DropWeapon_Implementation()
{
	DropAWeapon();
}


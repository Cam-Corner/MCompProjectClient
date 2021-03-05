// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Networking/MPCharacter.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		RHItemMesh(TEXT("StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Sword01SM.Sword01SM'"));

	_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandItem"));
	RootComponent = _Weapon;
	_Weapon->SetStaticMesh(RHItemMesh.Object);
	_Weapon->SetGenerateOverlapEvents(true);
	_Weapon->AddLocalRotation(FRotator(0, 0, -90));
	_Weapon->AddLocalOffset(FVector(-10, 3, 0));
	_Weapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	_Weapon->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	_Weapon->SetCollisionResponseToAllChannels(ECR_Overlap);
	//_Weapon->bMultiBodyOverlap = true;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	_Weapon->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::SwordHitBoxOverlapBegin);
	_Weapon->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::SwordHitBoxOverlapEnd);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}


void AWeaponBase::SwordHitBoxOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& HitResult)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		AMPCharacter* Player = Cast<AMPCharacter>(OtherActor);
		if (Player != NULL)
		{
			UE_LOG(LogTemp, Warning, TEXT("Added Player!"));
			_PlayersInSwordCol.Add(Player);
		}
	}
}

void AWeaponBase::SwordHitBoxOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		AMPCharacter* Player = Cast<AMPCharacter>(OtherActor);
		if (Player != NULL)
		{
			for (AMPCharacter* P : _PlayersInSwordCol)
			{
				if (P == Player)
				{
					UE_LOG(LogTemp, Warning, TEXT("Removed Player!"));
					_PlayersInSwordCol.Remove(P);
					break;
				}
			}
		}
	}
}


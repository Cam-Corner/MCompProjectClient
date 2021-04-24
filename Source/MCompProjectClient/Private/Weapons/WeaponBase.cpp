// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "Networking/MPPlayerController.h"
#include "GameFramework/Controller.h"
#include "Components/SphereComponent.h"
#include "Networking/MPCharacter.h"

DEFINE_LOG_CATEGORY(LogAWeaponBase);

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	//static ConstructorHelpers::FObjectFinder<UStaticMesh>
	//	RHItemMesh(TEXT("StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Sword01SM.Sword01SM'"));

	_Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	_Weapon->SetGenerateOverlapEvents(true);
	_Weapon->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	_Weapon->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	_Weapon->SetCollisionResponseToAllChannels(ECR_Ignore);
	_Weapon->bMultiBodyOverlap = true;
	RootComponent = _Weapon;
	

	_WeaponPickupHitbox = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphereCollision"));
	_WeaponPickupHitbox->SetupAttachment(RootComponent);
	_WeaponPickupHitbox->SetGenerateOverlapEvents(true);
	_WeaponPickupHitbox->SetSphereRadius(50.0f);
	_WeaponPickupHitbox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	_WeaponPickupHitbox->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	_WeaponPickupHitbox->SetCollisionResponseToAllChannels(ECR_Ignore);
	_WeaponPickupHitbox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Overlap);

	
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		_WeaponPickupHitbox->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::WeaponPickupOverlapBegin);
	}

	//_Weapon->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::SwordHitBoxOverlapBegin);
	//_Weapon->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::SwordHitBoxOverlapEnd);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority)
		return;

	if (!_bWeaponIsEquipedByPlayer)
	{
		SetActorRotation(FRotator(0, GetActorRotation().Yaw + 25 * DeltaTime, 0));
	}

}

void AWeaponBase::Attack()
{


}

void AWeaponBase::DropWeapon()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		FVector Start = GetActorLocation();
		FVector End = Start - FVector(0, 0, -10000);
		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECollisionChannel::ECC_Visibility))
		{
			SetActorLocation(Hit.Location + FVector(0, 0, 50.f));
		}
		
		SetOwner(NULL);
		_WeaponPickupHitbox->SetGenerateOverlapEvents(true);
		_WeaponPickupHitbox->SetSphereRadius(100);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		_CharacterAttachedTo = NULL;

		UE_LOG(LogAWeaponBase, Display, TEXT("Weapon Has Been Dropped"));

		_bWeaponIsEquipedByPlayer = false;
	}
}

void AWeaponBase::PickupWeapon(AController* NewOwner, AMPCharacter* CharacterToAttachTo, const FName SocketName)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		SetOwner(NewOwner);
		_WeaponPickupHitbox->SetGenerateOverlapEvents(false);
		_WeaponPickupHitbox->SetSphereRadius(0);

		AttachToActor(CharacterToAttachTo, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		//SetActorLocation(CharacterToAttachTo->GetMesh()->GetSocketLocation(SocketName));
		//SetActorRotation(CharacterToAttachTo->GetMesh()->GetSocketRotation(SocketName) + FRotator(0, 0, 90));

		UE_LOG(LogAWeaponBase, Display, TEXT("Weapon Has Been Picked up"));
		_CharacterAttachedTo = CharacterToAttachTo;
		_bWeaponIsEquipedByPlayer = true;
	}
}

void AWeaponBase::WeaponPickupOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& HitResult)
{
	if (!_bWeaponIsEquipedByPlayer)
	{
		if (AMPCharacter* MPC = Cast<AMPCharacter>(OtherActor))
		{
			MPC->PickupWeapon(this);
			//if (AController* PC = Cast<AController>(MPC->GetOwner()))
			//{
			//	PickupWeapon(PC, MPC);
			//}
		}
	}
}

void AWeaponBase::WeaponPickupOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

/*
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
*/


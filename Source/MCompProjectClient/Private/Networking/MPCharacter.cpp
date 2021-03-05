// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/MPCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Networking/MPCharacterAnimInstance.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectGlobals.h"
#include "Networking/MPGameState.h"
#include "Networking/MPPlayerState.h"
#include "Networking/MPPlayerController.h"
#include "Networking/MPGameMode.h"
#include "Net/UnrealNetwork.h"
#include "GameCamera.h"
#include "Components/SkinnedMeshComponent.h"
#include "Weapons/WeaponBase.h"

// Sets default values
AMPCharacter::AMPCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	/*===
	Sets up the skeletal mesh for the player
	===*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
			MeshAsset(TEXT("SkeletalMesh'/Game/ModularRPGHeroesPolyart/Meshes/OneMeshCharacters/SoldierSK.SoldierSK'"));

	static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass> 
			AnimInstanceBP(TEXT("AnimBlueprint'/Game/Blueprints/bp_CharacterAnim.bp_CharacterAnim_C'"));

	USkeletalMesh* PlayerMesh = MeshAsset.Object;
	GetMesh()->AnimClass = AnimInstanceBP.Object;
	GetMesh()->SetSkeletalMesh(PlayerMesh);
	GetMesh()->AddLocalOffset(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	/*===
	Sets up the static mesh for the right hand items
	===*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		RHItemMesh(TEXT("StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Sword01SM.Sword01SM'"));

	//_RightHandItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandItem"));
	//_RightHandItem->SetupAttachment(GetMesh(), "hand_rSocket");
	//_RightHandItem->SetStaticMesh(RHItemMesh.Object);
	//_RightHandItem->AddLocalRotation(FRotator(0, 0, -90));
	//_RightHandItem->AddLocalOffset(FVector(-10, 3, 0));
	//_RightHandItem->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//_RightHandItem->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	//_RightHandItem->SetCollisionResponseToAllChannels(ECR_Overlap);
	//_RightHandItem->bMultiBodyOverlap = true;


	/*===
	Sets up the static mesh for the Left hand items
	===*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		LHItemMesh(TEXT("StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Shield01SM.Shield01SM'"));

	_LeftHandItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHandItem"));
	_LeftHandItem->SetupAttachment(GetMesh(), "hand_lSocket");
	_LeftHandItem->SetStaticMesh(LHItemMesh.Object);
	_LeftHandItem->AddLocalRotation(FRotator(0, 0, 90));
	_LeftHandItem->AddLocalOffset(FVector(10, 0, 0));

	/*===
	Sets up the attack hitbox for the weapon
	===*/
	//_SwordHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordHitbox"));
	//_SwordHitBox->SetupAttachment(_RightHandItem);
}

// Called when the game starts or when spawned
void AMPCharacter::BeginPlay()
{
	Super::BeginPlay();

	//_RightHandItem->OnComponentBeginOverlap.AddDynamic(this, &AMPCharacter::SwordHitBoxOverlapBegin);
	//_RightHandItem->OnComponentEndOverlap.AddDynamic(this, &AMPCharacter::SwordHitBoxOverlapEnd);

	if (GetMesh()->GetAnimInstance() != NULL)
	{
		UMPCharacterAnimInstance* AnimI = Cast<UMPCharacterAnimInstance>(GetMesh()->GetAnimInstance());

		if (AnimI != NULL)
		{
			_AnimInstance = AnimI;
		}
	}

	_Weapon = GetWorld()->SpawnActor<AWeaponBase>();
	_Weapon->SetActorLocation(GetMesh()->GetSocketLocation("hand_rSocket"));
	//_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::, "hand_rSocket");
}

// Called every frame
void AMPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_Weapon->SetActorRotation(GetMesh()->GetSocketRotation("hand_rSocket") - FRotator(0, 0, 90));
	_Weapon->SetActorLocation(GetMesh()->GetSocketLocation("hand_rSocket"), true);



	if (!_bIsAttacking)
		FinalMovement();
	else
		CheckForHits();

	if (GetLocalRole() == ROLE_Authority)
	{
		if (_bIsAttacking)
		{
			if (_AttackDuration > 0)
				_AttackDuration -= DeltaTime;
			else
				Multicast_StopAttacking();
		}
	}

}

// Called to bind functionality to input
void AMPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Axis input bindings */
	PlayerInputComponent->BindAxis("MoveForward", this, &AMPCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMPCharacter::MoveRight);

	/* Action input bindings */
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Pressed, this, &AMPCharacter::StartedBlocking);
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Released, this, &AMPCharacter::StoppedBlocking);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AMPCharacter::StartAttacking);
}

void AMPCharacter::MoveUp(float Value)
{
	_FinalMovementDirection.X = Value;
}

void AMPCharacter::MoveRight(float Value)
{
	_FinalMovementDirection.Y = Value;
}

void AMPCharacter::StartAttacking()
{
	_bIsAttacking = true;

	if (_AnimInstance != NULL)
		_AnimInstance->SetIsAttacking(true);

	Server_SetAttacking(_bIsAttacking);
}

void AMPCharacter::StartedBlocking()
{
	_bBlocking = true;
	_AnimInstance->SetIsBlocking(_bBlocking);
	Server_SetEnabledBlock(_bBlocking);
}

void AMPCharacter::StoppedBlocking()
{
	_bBlocking = false;
	_AnimInstance->SetIsBlocking(_bBlocking);
	Server_SetEnabledBlock(_bBlocking);
}

void AMPCharacter::FinalMovement()
{

	if (_FinalMovementDirection.X + _FinalMovementDirection.Y > 1 
		|| _FinalMovementDirection.X + _FinalMovementDirection.Y < -1)
		_FinalMovementDirection.Normalize();

	//GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow, TEXT("Unit Vector: " + _FinalMovementDirection.ToCompactString()));

	float DotProduct = FVector::DotProduct(FVector::ForwardVector, _FinalMovementDirection);
	float Angle = acos(DotProduct);
	
	if (_FinalMovementDirection.Y < 0)
	{
		Angle = Angle * -1;
		Angle += 6.28;
	}

	//if (GetOwner() == UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetOwner())
	//{
	//	GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Yellow, TEXT("My Angle: " + FString::SanitizeFloat(Angle * 57.2958)));
	//}

	FRotator FinalRot = GetMesh()->GetComponentRotation();
	FinalRot.Yaw = (Angle * 57.2958) - 90;

	GetMesh()->SetWorldRotation(FinalRot);

	if (_AnimInstance != NULL)
	{

		if ((_FinalMovementDirection.X != 0 || _FinalMovementDirection.Y != 0) && !_bBlocking)
			_AnimInstance->SetCurrentSpeed(1);
		else
			_AnimInstance->SetCurrentSpeed(0);
	}

	Server_SetUnitVector(_FinalMovementDirection);

	AddMovementInput(_FinalMovementDirection, 1);
}

void AMPCharacter::CheckForHits()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Checking for Player hits!"));
		if (_Weapon->GetCharacterArray().Num() > 0)
		{
			for (AMPCharacter* P : _Weapon->GetCharacterArray())
			{
				AMPGameMode* GM = Cast<AMPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (GM != NULL)
				{
					UE_LOG(LogTemp, Warning, TEXT("In Loop"));
					TArray<AActor*> _PCs;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMPPlayerController::StaticClass(), _PCs);
					FHitInfo HitInfo;
					APlayerController* MyPC = Cast<APlayerController>(GetOwner());
					APlayerController* OtherPC = NULL;

					bool bFoundOwner = false;
					for (AActor* PC : _PCs)
					{
						OtherPC = Cast<APlayerController>(PC);

						if (OtherPC != NULL)
						{
							if (P->GetOwner() == OtherPC)
							{
								bFoundOwner = true;
								break;
							}
						}
					}

					if (bFoundOwner && OtherPC != NULL && MyPC != NULL)
					{
						GM->PlayerAttackedPlayer(this, MyPC, P, OtherPC, HitInfo);
						_Weapon->GetCharacterArray().Remove(P);
						break;
					}
				}
			}
		}
	}
}

bool AMPCharacter::Server_SetUnitVector_Validate(FVector UnitV)
{
	return true;
}

void AMPCharacter::Server_SetUnitVector_Implementation(FVector UnitV)
{
	_FinalMovementDirection = UnitV;
}

bool AMPCharacter::Server_SetEnabledBlock_Validate(bool bBlocked)
{
	return true;
}

void AMPCharacter::Server_SetEnabledBlock_Implementation(bool bBlocked)
{
	_bBlocking = bBlocked;
}

bool AMPCharacter::Server_SetAttacking_Validate(bool bAttacking)
{
	return true;
}

void AMPCharacter::Server_SetAttacking_Implementation(bool bAttacking)
{
	_bIsAttacking = bAttacking;
	_AttackDuration = 0.6f;
	UE_LOG(LogTemp, Warning, TEXT("Player started Attacking!"));

	if (_AnimInstance != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Anim!"));
		_AnimInstance->SetIsAttacking(true);
	}
}	

bool AMPCharacter::Multicast_StopAttacking_Validate()
{
	return true;
}

void AMPCharacter::Multicast_StopAttacking_Implementation()
{
	_AttackDuration = 0;
	_bIsAttacking = false;
	
	//_PlayersInSwordCol.Empty();

	if (_AnimInstance != NULL)
	{
		_AnimInstance->SetIsAttacking(false);
	}

}

void AMPCharacter::SwordHitBoxOverlapBegin(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& HitResult)
{
	/*if (GetLocalRole() == ROLE_Authority)
	{
		AMPCharacter* Player = Cast<AMPCharacter>(OtherActor);
		if (Player != NULL)
		{
			if (Player != this)
			{
				UE_LOG(LogTemp, Warning, TEXT("Added Player!"));
				_PlayersInSwordCol.Add(Player);
			}
		}
	}*/
}

void AMPCharacter::SwordHitBoxOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*if (GetLocalRole() == ROLE_Authority)
	{
		AMPCharacter* Player = Cast<AMPCharacter>(OtherActor);
		if (Player != NULL)
		{
			if (Player != this)
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
	}*/
}

void AMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPCharacter, _FinalMovementDirection);
	DOREPLIFETIME(AMPCharacter, _bBlocking);
	DOREPLIFETIME(AMPCharacter, _bIsAttacking);
	DOREPLIFETIME(AMPCharacter, _Health);
}


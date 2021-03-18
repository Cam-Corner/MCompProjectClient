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
#include "MPGameInstance.h"
#include "ItemData.h"

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
	//GetMesh()->SetSkeletalMesh(PlayerMesh);
	GetMesh()->AddLocalOffset(FVector(0, 0, -GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;

	/*===
	Setup the skeletal meshes for the body parts
	===*/
	_HelmetSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmetSM"));
	_HelmetSM->SetupAttachment(GetMesh());
	_HelmetSM->SetMasterPoseComponent(GetMesh());

	_HairSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairSM"));
	_HairSM->SetupAttachment(GetMesh());
	_HairSM->SetMasterPoseComponent(GetMesh());

	_FaceSM	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceSM"));
	_FaceSM->SetupAttachment(GetMesh());
	_FaceSM->SetMasterPoseComponent(GetMesh());

	_ShouldersSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShouldersSM"));
	_ShouldersSM->SetupAttachment(GetMesh());
	_ShouldersSM->SetMasterPoseComponent(GetMesh());

	_GlovesSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlovesSM"));
	_GlovesSM->SetupAttachment(GetMesh());
	_GlovesSM->SetMasterPoseComponent(GetMesh());

	_BeltSM	= CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BeltSM"));
	_BeltSM->SetupAttachment(GetMesh());
	_BeltSM->SetMasterPoseComponent(GetMesh());

	_ShoesSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoesSM"));
	_ShoesSM->SetupAttachment(GetMesh());
	_ShoesSM->SetMasterPoseComponent(GetMesh());


	/*===
	Sets up the static mesh for the right hand items
	===*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		RHItemMesh(TEXT("StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Sword01SM.Sword01SM'"));

	_RightHandItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightHandItem"));
	_RightHandItem->SetupAttachment(GetMesh());//, "hand_rSocket");
	_RightHandItem->SetStaticMesh(RHItemMesh.Object);
	_RightHandItem->AddLocalRotation(FRotator(0, 0, -90));
	_RightHandItem->AddLocalOffset(FVector(-10, 3, 0));
	_RightHandItem->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	_RightHandItem->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	_RightHandItem->SetCollisionResponseToAllChannels(ECR_Overlap);
	_RightHandItem->bMultiBodyOverlap = true;

	/*===
	Sets up the static mesh for the Left hand items
	===*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		LHItemMesh(TEXT("StaticMesh'/Game/ModularRPGHeroesPolyart/Meshes/Weapons/Shield01SM.Shield01SM'"));

	_LeftHandItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftHandItem"));
	_LeftHandItem->SetupAttachment(GetMesh());// , "hand_lSocket");
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

	if (GetLocalRole() == ROLE_Authority)
	{
		_RightHandItem->OnComponentBeginOverlap.AddDynamic(this, &AMPCharacter::SwordHitBoxOverlapBegin);
		_RightHandItem->OnComponentEndOverlap.AddDynamic(this, &AMPCharacter::SwordHitBoxOverlapEnd);

		UE_LOG(LogTemp, Display, TEXT("Request To Change Items!"));
		//RequestItems_Client();
	}
	else if(GetOwner() != UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		//
		SetupSkeletalCharacterMeshes(_EquipedGear._HelmetID, _EquipedGear._HairID, _EquipedGear._FaceID, 
			_EquipedGear._ShouldersID, _EquipedGear._BodyID, _EquipedGear._GlovesID, _EquipedGear._BeltID, 
			_EquipedGear._ShoesID);
	}
	else
	{
		if (UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			FFinalCharacterGear Gear = GI->GetEquipedItems();
			SetupSkeletalCharacterMeshes(Gear._HelmetID, Gear._HairID, Gear._FaceID, Gear._ShouldersID,
				Gear._BodyID, Gear._GlovesID, Gear._BeltID, Gear._ShoesID);

			GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Black, "Sending Items to Server: " + Gear._HelmetID);
			SendItemsEquiped_Server(Gear._HelmetID, Gear._HairID, Gear._FaceID, Gear._ShouldersID,
				Gear._BodyID, Gear._GlovesID, Gear._BeltID, Gear._ShoesID);
		}
	}

	EquipWeapon_Multicast();

	//_Weapon = GetWorld()->SpawnActor<AWeaponBase>();
	//_Weapon->SetActorLocation(GetMesh()->GetSocketLocation("hand_rSocket"));
	//_Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::, "hand_rSocket");
}

// Called every frame
void AMPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//_Weapon->SetActorRotation(GetMesh()->GetSocketRotation("hand_rSocket") - FRotator(0, 0, 90));
	//_Weapon->SetActorLocation(GetMesh()->GetSocketLocation("hand_rSocket"), true);

	if (GetLocalRole() == ROLE_Authority)
	{
		//
		//SetupSkeletalCharacterMeshes
		//SetupMeshes_Multicast(_EquipedGear._HelmetID, _EquipedGear._HairID, _EquipedGear._FaceID,
		//	_EquipedGear._ShouldersID, _EquipedGear._BodyID, _EquipedGear._GlovesID, _EquipedGear._BeltID,
		//	_EquipedGear._ShoesID);
	}

	if (!_bIsAttacking)
		FinalMovement();
	else
	{
		CheckForHits();
	
		//UE_LOG(LogTemp, Warning, TEXT("Sword Location: %s, Sword Rotation: %s"),
		//	*_Weapon->GetActorLocation().ToString(), *_Weapon->GetActorRotation().ToString());
	}
	
	if (GetLocalRole() == ROLE_Authority)
	{
		if (_bIsAttacking)
		{
			if (_AttackDuration > 0)
				_AttackDuration -= DeltaTime;
			else
			{
				Multicast_StopAttacking();
				_PlayersHitBySword.Empty();
			}
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
	//PlayerInputComponent->BindAction("Block", EInputEvent::IE_Released, this, &AMPCharacter::StoppedBlocking);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AMPCharacter::StartAttacking);
	PlayerInputComponent->BindAction("Quit", EInputEvent::IE_Pressed, this, &AMPCharacter::QuitGame);
}

void AMPCharacter::Restart()
{
	Super::Restart();

	if (GetLocalRole() == ROLE_Authority)
	{
		_Health = 100;
		_Stamina = 100;

		SetupSkeletalCharacterMeshes(_EquipedGear._HelmetID, _EquipedGear._HairID, _EquipedGear._FaceID,
			_EquipedGear._ShouldersID, _EquipedGear._BodyID, _EquipedGear._GlovesID, _EquipedGear._BeltID,
			_EquipedGear._ShoesID);

		EquipWeapon_Multicast();
	}
}

void AMPCharacter::QuitGame()
{ 
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->ConsoleCommand("quit"); 
}

void AMPCharacter::MoveUp(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move UP"));
	_FinalMovementDirection.X = Value;
}

void AMPCharacter::MoveRight(float Value)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move Right"));

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
	if(_bBlocking)
		_bBlocking = false;
	else
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
	if (_FinalMovementDirection.X > 0.5f || _FinalMovementDirection.X < -0.5f ||
		_FinalMovementDirection.Y > 0.5f || _FinalMovementDirection.Y < -0.5f)
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
			if (/*(_FinalMovementDirection.X != 0 || _FinalMovementDirection.Y != 0) &&*/ !_bBlocking)
			{
				//UE_LOG(LogTemp, Warning, TEXT("Setting Animation"));
				_AnimInstance->SetCurrentSpeed(1);
			}
			//else
				//_AnimInstance->SetCurrentSpeed(0);
		}

		Server_SetUnitVector(_FinalMovementDirection);

		AddMovementInput(_FinalMovementDirection, 1);
	}
	else
	{
		if (_AnimInstance != NULL)
		{
			_AnimInstance->SetCurrentSpeed(0);
		}
	}
}

void AMPCharacter::CheckForHits()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Checking for Player hits!"));


		if (_PlayersInSwordCol.Num() > 0)
		{
			for (AMPCharacter* P : _PlayersInSwordCol)
			{
				AMPGameMode* GM = Cast<AMPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
				if (GM != NULL)
				{
					//UE_LOG(LogTemp, Warning, TEXT("In Loop"));
					TArray<AActor*> _PCs;
					UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMPPlayerController::StaticClass(), _PCs);
					FHitInfo HitInfo;
					APlayerController* MyPC = Cast<APlayerController>(GetOwner());
					APlayerController* OtherPC = Cast<APlayerController>(P->GetOwner());//NULL;


					//bool bFoundOwner = false;
					//for (AActor* PC : _PCs)
					//{
					//	OtherPC = Cast<APlayerController>(PC);
					//
					//	if (OtherPC != NULL)
					//	{
					//		if (P->GetOwner() == OtherPC)
					//		{
					//			bFoundOwner = true;
					//			break;
					//		}
					//	}
					//}

					if (/*bFoundOwner && */OtherPC != NULL && MyPC != NULL)
					{
						bool bBeenHit = false;
						for (AMPCharacter* MPChar : _PlayersHitBySword)
						{
							if (MPChar == P)
							{
								bBeenHit = true;
								break;
							}
						}

						if (!bBeenHit)
						{
							GM->PlayerAttackedPlayer(this, MyPC, P, OtherPC, HitInfo);
							//_PlayersInSwordCol.Remove(P);
							_PlayersHitBySword.Add(P);
							break;
						}
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
	if (GetLocalRole() == ROLE_Authority)
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
	}
}

void AMPCharacter::SwordHitBoxOverlapEnd(class UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (GetLocalRole() == ROLE_Authority)
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
	}
}

bool AMPCharacter::SetupMeshes_Multicast_Validate(uint8 HelmetID, uint8 HairID, uint8 FaceID,
	uint8 ShouldersID, uint8 BodyID, uint8 GlovesID, uint8 BeltID, uint8 ShoesID
/*struct FFinalCharacterGear EquipedGear*/)
{

	return true;
}

void AMPCharacter::SetupMeshes_Multicast_Implementation(uint8 HelmetID, uint8 HairID, uint8 FaceID,
	uint8 ShouldersID, uint8 BodyID, uint8 GlovesID, uint8 BeltID, uint8 ShoesID
/*struct FFinalCharacterGear EquipedGear*/)
{
	UE_LOG(LogTemp, Error, TEXT("Multicast_Imp!: %i "), HelmetID);
	
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, "Multicast_SetupMesh!: " + HelmetID);

	if (GetLocalRole() == ROLE_Authority)
	{
		_EquipedGear._HelmetID = HelmetID;
		_EquipedGear._HairID = HairID;
		_EquipedGear._FaceID = FaceID;
		_EquipedGear._ShouldersID = ShouldersID;
		_EquipedGear._BodyID = BodyID;
		_EquipedGear._GlovesID = GlovesID;
		_EquipedGear._BeltID = BeltID;
		_EquipedGear._ShoesID = ShoesID;
	}

	SetupSkeletalCharacterMeshes(HelmetID, HairID, FaceID, ShouldersID, BodyID, GlovesID, BeltID, ShoesID);
}

void AMPCharacter::SetupSkeletalCharacterMeshes(uint8 HelmetID, uint8 HairID, uint8 FaceID,
	uint8 ShouldersID, uint8 BodyID, uint8 GlovesID, uint8 BeltID, uint8 ShoesID
/*struct FFinalCharacterGear EquipedGear*/)
{	
	if (UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GetMesh()->SetSkeletalMesh(GI->GetItemTable()->GetBodyByID(BodyID));
		_HelmetSM->SetSkeletalMesh(GI->GetItemTable()->GetHelmetByID(HelmetID));
		_HairSM->SetSkeletalMesh(GI->GetItemTable()->GetHairByID(HairID));
		_FaceSM->SetSkeletalMesh(GI->GetItemTable()->GetFaceByID(FaceID));
		_ShouldersSM->SetSkeletalMesh(GI->GetItemTable()->GetShouldersByID(ShouldersID));
		_GlovesSM->SetSkeletalMesh(GI->GetItemTable()->GetGlovesByID(GlovesID));
		_BeltSM->SetSkeletalMesh(GI->GetItemTable()->GetBeltByID(BeltID));
		_ShoesSM->SetSkeletalMesh(GI->GetItemTable()->GetShoesByID(ShoesID));

		if (_AnimInstance == NULL)
		{
			if (GetMesh()->GetAnimInstance() != NULL)
			{
				UMPCharacterAnimInstance* AnimI = Cast<UMPCharacterAnimInstance>(GetMesh()->GetAnimInstance());

				if (AnimI != NULL)
				{
					UE_LOG(LogTemp, Warning, TEXT("Caching Anim Instance"));
					_AnimInstance = AnimI;
				}
				else
					UE_LOG(LogTemp, Warning, TEXT("AnimI Cast NULL"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Get Anim Instance NULL"));
			}
		}
	}
}

bool AMPCharacter::SendItemsEquiped_Server_Validate(uint8 HelmetID, uint8 HairID, uint8 FaceID,
	uint8 ShouldersID, uint8 BodyID, uint8 GlovesID, uint8 BeltID, uint8 ShoesID
/*struct FFinalCharacterGear EquipedGear*/)
{

	return true;
}

void AMPCharacter::SendItemsEquiped_Server_Implementation(uint8 HelmetID, uint8 HairID, uint8 FaceID,
	uint8 ShouldersID, uint8 BodyID, uint8 GlovesID, uint8 BeltID, uint8 ShoesID
/*struct FFinalCharacterGear EquipedGear*/)
{
	UE_LOG(LogTemp, Error, TEXT("ItemEquiped_imp!: %i "), HelmetID);
	SetupMeshes_Multicast(HelmetID, HairID, FaceID, ShouldersID, BodyID, GlovesID, BeltID, ShoesID);
}

bool AMPCharacter::RequestItems_Client_Validate()
{

	return true;
}

void AMPCharacter::RequestItems_Client_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Error, TEXT("Am the server!"));
		return;
	}

	GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Red, "ServerRequestedItems");
	UE_LOG(LogTemp, Error, TEXT("ServerRequestedItems"));

	if (UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		FFinalCharacterGear Gear = GI->GetEquipedItems();
		GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Black, "ServerRequestedItems: " + Gear._HelmetID);
		UE_LOG(LogTemp, Error, TEXT("ItemEquiped_imp!: %i "), Gear._HelmetID);
		SendItemsEquiped_Server(Gear._HelmetID, Gear._HairID, Gear._FaceID, Gear._ShouldersID,
			Gear._BodyID, Gear._GlovesID, Gear._BeltID, Gear._ShoesID);
	}
}

bool AMPCharacter::EquipWeapon_Multicast_Validate()
{

	return true;
}

void AMPCharacter::EquipWeapon_Multicast_Implementation()
{
	if (_LeftHandItem != NULL)
	{
		_LeftHandItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_lSocket");
		_LeftHandItem->AddLocalRotation(FRotator(0, 0, 90));
		_LeftHandItem->AddLocalOffset(FVector(10, 0, 0));
	}

	if (_RightHandItem != NULL)
	{
		_RightHandItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "hand_rSocket");
		_RightHandItem->AddLocalRotation(FRotator(0, 0, -90));
		_RightHandItem->AddLocalOffset(FVector(-10, 3, 0));
	}
}

void AMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPCharacter, _FinalMovementDirection);
	DOREPLIFETIME(AMPCharacter, _bBlocking);
	DOREPLIFETIME(AMPCharacter, _bIsAttacking);
	DOREPLIFETIME(AMPCharacter, _Health);
	DOREPLIFETIME(AMPCharacter, _EquipedGear);
}


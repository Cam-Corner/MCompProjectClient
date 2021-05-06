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
#include "Networking/MPCamera.h"
#include "Net/UnrealNetwork.h"
#include "GameCamera.h"
#include "UI/GameHUD.h"
#include "Components/SkinnedMeshComponent.h"
#include "Weapons/WeaponBase.h"
#include "MPGameInstance.h"
#include "ItemData.h"
//#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogAMPCharacter);
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
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/* Temporary set all the meshes to the default (0 index)
	*  until the server recieves the right equipment */
	SetupSkeletalCharacterMeshes(0, 0, 0, 0, 0, 0, 0, 0); 
	
	_LeftHandItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "LeftWeaponShield");
	_RightHandItem->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "RightWeaponShield");
	
	if (GetLocalRole() == ROLE_Authority)
	{
		/* Sword hit events are used to check if the sword hit a player when attacking
		   only needs to do this on the server since the client should never handle the attack events */
		_RightHandItem->OnComponentBeginOverlap.AddDynamic(this, &AMPCharacter::SwordHitBoxOverlapBegin);
		_RightHandItem->OnComponentEndOverlap.AddDynamic(this, &AMPCharacter::SwordHitBoxOverlapEnd);
		

		
		UE_LOG(LogAMPCharacter, Display, TEXT("Request To Change Items!"));
		//RequestItems_Client();

		//EquipWeapon_Multicast();
	}
	else if(GetOwner() != UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		//
		SetupSkeletalCharacterMeshes(_EquipedGear._HelmetID, _EquipedGear._HairID, _EquipedGear._FaceID, 
			_EquipedGear._ShouldersID, _EquipedGear._BodyID, _EquipedGear._GlovesID, _EquipedGear._BeltID, 
			_EquipedGear._ShoesID);

		if (AMPPlayerController* PC = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			//PC->Server_UpdateItems();
		}

		Server_EquipWeaponProperly();
	}
	else
	{
		if (UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
		{
			FFinalCharacterGear Gear = GI->GetEquipedItems();
			SetupSkeletalCharacterMeshes(Gear._HelmetID, Gear._HairID, Gear._FaceID, Gear._ShouldersID,
				Gear._BodyID, Gear._GlovesID, Gear._BeltID, Gear._ShoesID);

			//GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Black, "Sending Items to Server: " + Gear._HelmetID);
			SendItemsEquiped_Server(Gear._HelmetID, Gear._HairID, Gear._FaceID, Gear._ShouldersID,
				Gear._BodyID, Gear._GlovesID, Gear._BeltID, Gear._ShoesID);
		}
		Server_EquipWeaponProperly();

	}

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


	//if (_TempTimer <= 0)
	//{
	//	UE_LOG(LogAMPCharacter, Warning, TEXT("Actor Rotation: %f"), GetActorRotation().Yaw);
	//	UE_LOG(LogAMPCharacter, Warning, TEXT("Mesh Rotation: %f"), GetMesh()->GetComponentRotation().Yaw);
	//
	//	_TempTimer = 0.25f;
	//}
	//else
	//	_TempTimer -= DeltaTime;

	/*if (_KnockBackAmount > 0)
	{
		//GetCharacterMovement()->MaxWalkSpeed = 2000;
		//Move Player Backwards
		//if (GetLocalRole() != ROLE_Authority && 
		//	GetOwner() == UGameplayStatics::GetPlayerController(GetWorld(), 0))						
		//	//AddMovementInput(FVector(_KnockBackDir, 0), _KnockBackSpeedMultiplier);
		//if (GetLocalRole() == ROLE_Authority)
		//check how much they have moved
		FVector2D NewPos;
		NewPos.X = GetActorLocation().X;
		NewPos.Y = GetActorLocation().Y;
		
		float Distance = FVector2D::Distance(NewPos, OldPos);
		
		_KnockBackAmount -= Distance;
		
		FString DebugMessage = "KnockBack Amount: " + FString::SanitizeFloat(_KnockBackAmount) +
			" || Distance: " + FString::SanitizeFloat(Distance) + 
			" || OldPos: " + OldPos.ToString() + " || NewPos:" + NewPos.ToString();
		
		if(GetLocalRole() != ROLE_Authority)
			GEngine->AddOnScreenDebugMessage(5, 1.0f, FColor::Yellow, DebugMessage);
		
		_bBlocking = false;
		_bIsAttacking = false;
		
		SmoothRotationToDirection(DeltaTime);
		
		//Save old pos
		OldPos = NewPos;
		return;
	}*/

	if (GetLocalRole() == ROLE_Authority)
	{
		//
		//SetupSkeletalCharacterMeshes
		//SetupMeshes_Multicast(_EquipedGear._HelmetID, _EquipedGear._HairID, _EquipedGear._FaceID,
		//	_EquipedGear._ShouldersID, _EquipedGear._BodyID, _EquipedGear._GlovesID, _EquipedGear._BeltID,
		//	_EquipedGear._ShoesID);


		if (_bIsAttacking)
		{
			//UE_LOG(LogAMPCharacter, Warning, TEXT("Sword Location: %s, Sword Rotation: %s"),
			//	*_Weapon->GetActorLocation().ToString(), *_Weapon->GetActorRotation().ToString());
			CheckForHits();

			if (_AttackDuration > 0)
				_AttackDuration -= DeltaTime;
			else
			{
				Multicast_StopAttacking();
				_PlayersHitBySword.Empty();
			}
		}
		else
		{
			//FinalMovement(DeltaTime);
		}

		SmoothRotationToDirection(DeltaTime);
	}
	else if (GetOwner() == UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (!_bIsAttacking)
			FinalMovement(DeltaTime);

		SmoothRotationToDirection(DeltaTime);

		if (AGameHUD* HUD = Cast<AGameHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD()))
		{
			HUD->SetHealthBar(_Health, 100);
		}
	}
	else if (GetOwner() != UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (_AnimInstance != NULL)
		{
			_AnimInstance->SetIsAttacking(_bIsAttacking);
			_AnimInstance->SetIsBlocking(_bBlocking);

			if (_FinalMovementDirection.X != 0 || _FinalMovementDirection.Y != 0)
				_AnimInstance->SetCurrentSpeed(1);
			else
				_AnimInstance->SetCurrentSpeed(0);
		}

		SmoothRotationToDirection(DeltaTime);
	}
}

/*void AMPCharacter::Multicast_SendPlayersMyEquiptment_Implementation()
{
	SetupSkeletalCharacterMeshes(_EquipedGear._HelmetID, _EquipedGear._HairID, _EquipedGear._FaceID,
		_EquipedGear._ShouldersID, _EquipedGear._BodyID, _EquipedGear._GlovesID, _EquipedGear._BeltID,
		_EquipedGear._ShoesID);

}*/

// Called to bind functionality to input
void AMPCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Axis input bindings */
	PlayerInputComponent->BindAxis("MoveForward", this, &AMPCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMPCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMPCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AMPCharacter::LookRight);

	/* Action input bindings */
	PlayerInputComponent->BindAction("Block", EInputEvent::IE_Pressed, this, &AMPCharacter::StartedBlocking);
	//PlayerInputComponent->BindAction("Block", EInputEvent::IE_Released, this, &AMPCharacter::StoppedBlocking);
	PlayerInputComponent->BindAction("Attack", EInputEvent::IE_Pressed, this, &AMPCharacter::StartAttacking);
	PlayerInputComponent->BindAction("Quit", EInputEvent::IE_Pressed, this, &AMPCharacter::QuitGame);
}

void AMPCharacter::Multicast_KnockBack_Implementation(float Distance, FVector2D Direction)
{
	_KnockBackDir = Direction;
	_KnockBackAmount = Distance;
	LaunchCharacter(FVector(_KnockBackDir * (_KnockBackSpeedMultiplier), 0), true, true);
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
	//UE_LOG(LogAMPCharacter, Warning, TEXT("Move UP"));
	_FinalMovementDirection.X = Value;
}

void AMPCharacter::MoveRight(float Value)
{
	//UE_LOG(LogAMPCharacter, Warning, TEXT("Move Right"));

	_FinalMovementDirection.Y = Value;
}

void AMPCharacter::LookUp(float Value)
{
	if(GetOwner() != UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Looking Up"));
	}

	_LookDirection.X = Value;
}

void AMPCharacter::LookRight(float Value)
{
	_LookDirection.Y = Value;
}

void AMPCharacter::StartAttacking()
{
	_bIsAttacking = true;

	if (_AnimInstance != NULL)
		_AnimInstance->SetIsAttacking(true);

	if (_SpecialWeaponEquiped != NULL)
	{
		_SpecialWeaponEquiped->Attack();
	}

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

void AMPCharacter::FinalMovement(float DeltaTime)
{
	if (_FinalMovementDirection.X > 0.5f || _FinalMovementDirection.X < -0.5f ||
		_FinalMovementDirection.Y > 0.5f || _FinalMovementDirection.Y < -0.5f)
	{
		if (_FinalMovementDirection.X + _FinalMovementDirection.Y > 1
			|| _FinalMovementDirection.X + _FinalMovementDirection.Y < -1)
			_FinalMovementDirection.Normalize();

		_LastMovementDirection = _FinalMovementDirection;

		{
			//float DotProduct = FVector::DotProduct(FVector::ForwardVector, _FinalMovementDirection);
			//float Angle = acos(DotProduct);
			//
			//if (_FinalMovementDirection.Y < 0)
			//{
			//	Angle = Angle * -1;
			//	Angle += 6.28;
			//}

			//if (GetOwner() == UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetOwner())
			//{
			//	GEngine->AddOnScreenDebugMessage(3, 5.0f, FColor::Yellow, TEXT("My Angle: " + FString::SanitizeFloat(Angle * 57.2958)));
			//}

			//FRotator FinalRot = GetMesh()->GetComponentRotation();
			//FinalRot.Yaw = FMath::Lerp(FinalRot.Yaw,(float)((Angle * 57.2958) - 90), 0.5f);
			//
		}

		if (_AnimInstance != NULL)
		{
			if (!_bBlocking)
			{
				//UE_LOG(LogAMPCharacter, Warning, TEXT("Setting Animation"));
				_AnimInstance->SetCurrentSpeed(1);
			}
		}
		
		if(GetOwner() == UGameplayStatics::GetPlayerController(GetWorld(), 0))
			Server_SetUnitVector(_FinalMovementDirection);

		AddMovementInput(/*_FinalMovementDirection*/ FVector(_LastMovementDirection.X, _LastMovementDirection.Y, 0), 1);
	}
	else
	{
		if (_AnimInstance != NULL)
		{
			if (GetOwner() == UGameplayStatics::GetPlayerController(GetWorld(), 0))
				Server_SetUnitVector(_FinalMovementDirection);

			_AnimInstance->SetCurrentSpeed(0);
		}
	}
}

void AMPCharacter::SmoothRotationToDirection(float DeltaTime)
{
	if (_bIsAttacking)
		return;

	if (GetOwner() != UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		//GEngine->AddOnScreenDebugMessage(7, 5.0f, FColor::Blue, _LookDirection.ToString());

		if (_FinalMovementDirection.X != 0 || _FinalMovementDirection.Y != 0)
		{
			_LastMovementDirection = _FinalMovementDirection;
		}
	}

	float GotoYaw = 0;

	if (_LookDirection.X != 0 || _LookDirection.Y != 0)
	{
		GotoYaw = atan2(_LookDirection.Y, _LookDirection.X);
		_LastMovementDirection.X = _LookDirection.X;
		_LastMovementDirection.Y = _LookDirection.Y;
	}
	else
		GotoYaw = atan2(_LastMovementDirection.Y, _LastMovementDirection.X);
	
	GotoYaw = GotoYaw * 57.2958; //convert to degrees

	if (GotoYaw < 0) GotoYaw += 360;

	float LerpSpeed = 10.0f;
	if (CurrentYaw - GotoYaw > 180 || CurrentYaw - GotoYaw < -180)
	{
		if (CurrentYaw > 180)
		{
			CurrentYaw = FMath::Lerp(CurrentYaw, 360.0f + GotoYaw, DeltaTime * LerpSpeed);

			if (CurrentYaw >= 360)
				CurrentYaw = FMath::Fmod(CurrentYaw, 360);
		}
		else
		{
			CurrentYaw = FMath::Lerp(CurrentYaw, 0.0f - (360 - GotoYaw), DeltaTime * LerpSpeed);

			if (CurrentYaw < 0)
			{
				CurrentYaw = 360 - FMath::Fmod(CurrentYaw, 360);
			}
		}
	}
	else
		CurrentYaw = FMath::Lerp(CurrentYaw, GotoYaw, DeltaTime * LerpSpeed);

	//GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Yellow, TEXT("My Angle: " + FString::SanitizeFloat(DeltaTime)));

	SetActorRotation(FRotator(GetActorRotation().Pitch, CurrentYaw, GetActorRotation().Roll));
	GetMesh()->SetWorldRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw - 90, GetActorRotation().Roll));

	if (GetLocalRole() != ROLE_Authority && GetOwner() == UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		Server_SetRotation(GetActorRotation());
		Server_SetLookDirection(_LookDirection);
	}
}

void AMPCharacter::PickupWeapon(AWeaponBase* Weapon)
{
	if (GetController() == NULL || Weapon == NULL || _SpecialWeaponEquiped != NULL)
		return;

	//Weapon->PickupWeapon(GetController(), this, "LeftWeaponShield");

	//if (_SpecialWeaponEquiped != NULL)
	//{
	//	DropWeapons();
	//}

	Multicast_EquipNewWeapon(Weapon);
}

void AMPCharacter::DropWeapons()
{
	if(_SpecialWeaponEquiped != NULL)
		_SpecialWeaponEquiped->DropWeapon();

	//_LeftHandItem->SetVisibility(true);
	//_RightHandItem->SetVisibility(true);
	Multicast_EquipNewWeapon(NULL);
}

void AMPCharacter::CheckForHits()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		//UE_LOG(LogAMPCharacter, Warning, TEXT("Checking for Player hits!"));


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

void AMPCharacter::Server_SetRotation_Implementation(FRotator Rotation)
{
	SetActorRotation(Rotation);
	GetMesh()->SetWorldRotation(FRotator(Rotation.Pitch, Rotation.Yaw - 90, Rotation.Roll));
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
	if (_SpecialWeaponEquiped == NULL)
	{
		_bIsAttacking = bAttacking;
		_AttackDuration = 0.6f;
	}
	else
	{
		_bIsAttacking = bAttacking;
		_AttackDuration = 0.6f;
		_SpecialWeaponEquiped->Attack();
	}

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
	
	//GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, "Multicast_SetupMesh!: " + HelmetID);

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

	//GEngine->AddOnScreenDebugMessage(1, 15.0f, FColor::Red, "ServerRequestedItems");
	UE_LOG(LogTemp, Error, TEXT("ServerRequestedItems"));

	if (UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		FFinalCharacterGear Gear = GI->GetEquipedItems();
		//GEngine->AddOnScreenDebugMessage(2, 15.0f, FColor::Black, "ServerRequestedItems: " + Gear._HelmetID);
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
	if (GetLocalRole() != ROLE_Authority)
		return;

	if (_LeftHandItem != NULL)
	{		
		//_LeftHandItem->SetWorldLocation(GetMesh()->GetBoneLocation("hand_lSocket"), EBoneSpaces::WorldSpace);
		//_LeftHandItem->SetWorldRotation(GetMesh()->GetBoneQuaternion("hand_lSocket"), EBoneSpaces::WorldSpace);
		//_LeftHandItem->AddLocalRotation(FRotator(0, 0, 90));
		//_LeftHandItem->AddLocalOffset(FVector(10, 0, 0));
	}

	if (_RightHandItem != NULL)
	{
		//_RightHandItem->SetWorldLocation(GetMesh()->GetComponentLocation());//->GetBoneLocation("hand_rSocket"), EBoneSpaces::WorldSpace);
		//_RightHandItem->SetWorldRotation(GetMesh()->GetBoneQuaternion());//->GetBoneQuaternion("hand_rSocket"), EBoneSpaces::WorldSpace);
		//_RightHandItem->AddLocalRotation(FRotator(0, 0, -90));
		//_RightHandItem->AddLocalOffset(FVector(-10, 3, 0));
	}
}

bool AMPCharacter::Server_SetLookDirection_Validate(FVector2D LookDirection)
{
	return true;
}

void AMPCharacter::Server_SetLookDirection_Implementation(FVector2D LookDirection)
{
	_LookDirection = LookDirection;
}

bool AMPCharacter::Multicast_EquipNewWeapon_Validate(AWeaponBase* Weapon)
{
	return true;
}

void AMPCharacter::Multicast_EquipNewWeapon_Implementation(AWeaponBase* Weapon)
{
	if (Weapon != NULL)
	{
		_LeftHandItem->SetVisibility(false);
		_RightHandItem->SetVisibility(false);
		_SpecialWeaponEquiped = Weapon;

		if (_AnimInstance != NULL)
		{
			_AnimInstance->SetWeaponType(1);
		}

		Weapon->PickupWeapon(GetController(), this, "LeftWeaponShield");
	}
	else
	{
		_LeftHandItem->SetVisibility(true);
		_RightHandItem->SetVisibility(true);
		_SpecialWeaponEquiped = NULL;

		if (_AnimInstance != NULL)
		{
			_AnimInstance->SetWeaponType(0);
		}
	}
}

bool AMPCharacter::Server_EquipWeaponProperly_Validate()
{
	return true;
}

void AMPCharacter::Server_EquipWeaponProperly_Implementation()
{
	//EquipWeapon_Multicast();

}

void AMPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMPCharacter, _FinalMovementDirection);
	DOREPLIFETIME(AMPCharacter, _LookDirection);
	DOREPLIFETIME(AMPCharacter, _bBlocking);
	DOREPLIFETIME(AMPCharacter, _bIsAttacking);
	DOREPLIFETIME(AMPCharacter, _Health);
	DOREPLIFETIME(AMPCharacter, _EquipedGear);
	//DOREPLIFETIME(AMPCharacter, _HelmetID);
	/*DOREPLIFETIME(AMPCharacter, _HairID);
	DOREPLIFETIME(AMPCharacter, _FaceID);
	DOREPLIFETIME(AMPCharacter, _ShouldersID);
	DOREPLIFETIME(AMPCharacter, _GlovesID);
	DOREPLIFETIME(AMPCharacter, _BeltID);
	DOREPLIFETIME(AMPCharacter, _ShoesID);
	DOREPLIFETIME(AMPCharacter, _BodyID);*/

}
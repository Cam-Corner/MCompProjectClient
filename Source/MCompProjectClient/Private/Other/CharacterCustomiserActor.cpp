// Fill out your copyright notice in the Description page of Project Settings.


#include "Other/CharacterCustomiserActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "MPGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ItemData.h"
#include "Engine/SkeletalMesh.h"
#include "MPGameInstance.h"

// Sets default values
ACharacterCustomiserActor::ACharacterCustomiserActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//don't need tick event because all changes will be from functions called
	PrimaryActorTick.bCanEverTick = false;

	//static ConstructorHelpers::FObjectFinder<UAnimBlueprintGeneratedClass>
	//	AnimInstanceBP(TEXT("AnimBlueprint'/Game/Blueprints/bp_CharacterAnim.bp_CharacterAnim_C'"));

	_BodySM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	//_BodySM->AnimClass = AnimInstanceBP.Object;
	RootComponent = _BodySM;

	_HelmetSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HelmetMesh"));
	_HelmetSM->SetupAttachment(_BodySM);
	_HelmetSM->SetMasterPoseComponent(_BodySM);

	_HairSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	_HairSM->SetupAttachment(_BodySM);
	_HairSM->SetMasterPoseComponent(_BodySM);

	_FaceSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FaceMesh"));
	_FaceSM->SetupAttachment(_BodySM);
	_FaceSM->SetMasterPoseComponent(_BodySM);

	_ShouldersSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShouldersMesh"));
	_ShouldersSM->SetupAttachment(_BodySM);
	_ShouldersSM->SetMasterPoseComponent(_BodySM);

	_GlovesSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GlovesMesh"));
	_GlovesSM->SetupAttachment(_BodySM);
	_GlovesSM->SetMasterPoseComponent(_BodySM);

	_BeltSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BeltMesh"));
	_BeltSM->SetupAttachment(_BodySM);
	_BeltSM->SetMasterPoseComponent(_BodySM);

	_ShoesSM = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShoesMesh"));
	_ShoesSM->SetupAttachment(_BodySM);
	_ShoesSM->SetMasterPoseComponent(_BodySM);
}

// Called when the game starts or when spawned
void ACharacterCustomiserActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true);

	UpdateBodyParts();	
}

void ACharacterCustomiserActor::IncreaseArmourInt(uint8 Type)
{
	if (Type == 0 || Type > 8)
		return;

	if (UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (Type == 1)
			_CurrentHeadID = NewCurrentGearID(_CurrentHeadID, GI->GetItemTable()->GetAmountOfHelmets(), true);
		else if(Type == 2)
			_CurrentHairID = NewCurrentGearID(_CurrentHairID, GI->GetItemTable()->GetAmountOfHair(), true);
		else if (Type == 3)
			_CurrentFaceID = NewCurrentGearID(_CurrentFaceID, GI->GetItemTable()->GetAmountOfFace(), true);
		else if (Type == 4)
			_CurrentShouldersID = NewCurrentGearID(_CurrentShouldersID, GI->GetItemTable()->GetAmountOfShoulders(), true);
		else if (Type == 5)
			_CurrentBodyID = NewCurrentGearID(_CurrentBodyID, GI->GetItemTable()->GetAmountOfBody(), true);
		else if (Type == 6)
			_CurrentGlovesID = NewCurrentGearID(_CurrentGlovesID, GI->GetItemTable()->GetAmountOfGloves(), true);
		else if (Type == 7)
			_CurrentBeltID = NewCurrentGearID(_CurrentBeltID, GI->GetItemTable()->GetAmountOfBelt(), true);
		else if (Type == 8)
			_CurrentShoesID = NewCurrentGearID(_CurrentShoesID, GI->GetItemTable()->GetAmountOfShoes(), true);

		UE_LOG(LogTemp, Error, TEXT("Armour Increased!"));
	}

	UpdateBodyParts();
}

uint8 ACharacterCustomiserActor::NewCurrentGearID(const uint8& CurrentID, uint8 MaxID, bool bIncreasing)
{
	uint8 ID = CurrentID;

	if (bIncreasing)
		ID++;
	else
		ID--;

	if (ID >= MaxID)
		ID = 0;
	
	if (ID < 0)
		ID = MaxID - 1;

	return ID;
}

void ACharacterCustomiserActor::DecreaseArmourInt(uint8 Type)
{
	if (Type == 0 || Type > 8)
		return;

	if (UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (Type == 1)
			_CurrentHeadID = NewCurrentGearID(_CurrentHeadID, GI->GetItemTable()->GetAmountOfHelmets(), false);
		else if (Type == 2)
			_CurrentHairID = NewCurrentGearID(_CurrentHairID, GI->GetItemTable()->GetAmountOfHair(), false);
		else if (Type == 3)
			_CurrentFaceID = NewCurrentGearID(_CurrentFaceID, GI->GetItemTable()->GetAmountOfFace(), false);
		else if (Type == 4)
			_CurrentShouldersID = NewCurrentGearID(_CurrentShouldersID, GI->GetItemTable()->GetAmountOfShoulders(), false);
		else if (Type == 5)
			_CurrentBodyID = NewCurrentGearID(_CurrentBodyID, GI->GetItemTable()->GetAmountOfBody(), false);
		else if (Type == 6)
			_CurrentGlovesID = NewCurrentGearID(_CurrentGlovesID, GI->GetItemTable()->GetAmountOfGloves(), false);
		else if (Type == 7)
			_CurrentBeltID = NewCurrentGearID(_CurrentBeltID, GI->GetItemTable()->GetAmountOfBelt(), false);
		else if (Type == 8)
			_CurrentShoesID = NewCurrentGearID(_CurrentShoesID, GI->GetItemTable()->GetAmountOfShoes(), false);

		UE_LOG(LogTemp, Error, TEXT("Armour Decreased!"));
	}

	UpdateBodyParts();
}

void ACharacterCustomiserActor::UpdateBodyParts()
{
	if (UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		UE_LOG(LogTemp, Error, TEXT("Set New Meshes! %i"), _CurrentHeadID);
		_HelmetSM->SetSkeletalMesh(GI->GetItemTable()->GetHelmetByID(_CurrentHeadID));
		_HairSM->SetSkeletalMesh(GI->GetItemTable()->GetHairByID(_CurrentHairID));
		_FaceSM->SetSkeletalMesh(GI->GetItemTable()->GetFaceByID(_CurrentFaceID));
		_ShouldersSM->SetSkeletalMesh(GI->GetItemTable()->GetShouldersByID(_CurrentShouldersID));
		_BodySM->SetSkeletalMesh(GI->GetItemTable()->GetBodyByID(_CurrentBodyID));
		_GlovesSM->SetSkeletalMesh(GI->GetItemTable()->GetGlovesByID(_CurrentGlovesID));
		_BeltSM->SetSkeletalMesh(GI->GetItemTable()->GetBeltByID(_CurrentBeltID));
		_ShoesSM->SetSkeletalMesh(GI->GetItemTable()->GetShoesByID(_CurrentShoesID));
	

		FFinalCharacterGear FinalGear;
		FinalGear._HelmetID = _CurrentHeadID;
		FinalGear._HairID = _CurrentHairID;
		FinalGear._FaceID = _CurrentFaceID;
		FinalGear._ShouldersID = _CurrentShouldersID;
		FinalGear._BodyID = _CurrentBodyID;
		FinalGear._GlovesID = _CurrentGlovesID;
		FinalGear._BeltID = _CurrentBeltID;
		FinalGear._ShoesID = _CurrentShoesID;

		GI->SetEquipedGear(FinalGear);
	}
}

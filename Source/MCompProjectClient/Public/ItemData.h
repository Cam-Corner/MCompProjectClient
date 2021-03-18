// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"
/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UItemData();

private:
	UPROPERTY(EditAnywhere, Category = "Items")
	TArray<TSubclassOf<class UItemBase>> _HeadItems;

	UPROPERTY(EditAnywhere, Category = "Items")
		TArray<TSubclassOf<class UItemBase>> _HairItems;

	UPROPERTY(EditAnywhere, Category = "Items")
		TArray<TSubclassOf<class UItemBase>> _FaceItems;

	UPROPERTY(EditAnywhere, Category = "Items")
		TArray<TSubclassOf<class UItemBase>> _ShouldersItems;

	UPROPERTY(EditAnywhere, Category = "Items")
		TArray<TSubclassOf<class UItemBase>> _BodyItems;

	UPROPERTY(EditAnywhere, Category = "Items")
		TArray<TSubclassOf<class UItemBase>> _GlovesItems;

	UPROPERTY(EditAnywhere, Category = "Items")
		TArray<TSubclassOf<class UItemBase>> _BeltItems;

	UPROPERTY(EditAnywhere, Category = "Items")
		TArray<TSubclassOf<class UItemBase>> _ShoesItems;

public:
	/**
	Returns the amount of helmets in the TArray
	*/
	const uint8 GetAmountOfHelmets() { return (uint8)_HeadItems.Num(); }

	/**
	Returns the amount of helmets in the TArray
	*/
	const uint8 GetAmountOfHair() { return (uint8)_HairItems.Num(); }

	/**
	Returns the amount of helmets in the TArray
	*/
	const uint8 GetAmountOfFace() { return (uint8)_FaceItems.Num(); }

	/**
	Returns the amount of helmets in the TArray
	*/
	const uint8 GetAmountOfShoulders() { return (uint8)_ShouldersItems.Num(); }

	/**
	Returns the amount of helmets in the TArray
	*/
	const uint8 GetAmountOfBody() { return (uint8)_BodyItems.Num(); }

	/**
	Returns the amount of helmets in the TArray
	*/
	const uint8 GetAmountOfGloves() { return (uint8)_GlovesItems.Num(); }

	/**
	Returns the amount of helmets in the TArray
	*/
	const uint8 GetAmountOfBelt() { return (uint8)_BeltItems.Num(); }

	/**
	Returns the amount of helmets in the TArray
	*/
	const uint8 GetAmountOfShoes() { return (uint8)_ShoesItems.Num(); }

	/**
	*Returns a skeletal mesh based on the ID recieved
	*@param Could return NULL (if casts fail) but shouldnt
	*@param if ID is bigger than the vector ID returns the last item in the list to prevent null ID
	*/
	class USkeletalMesh* GetHelmetByID(uint8 ID);

	/**
	*Returns a skeletal mesh based on the ID recieved
	*@param Could return NULL (if casts fail) but shouldnt
	*@param if ID is bigger than the vector ID returns the last item in the list to prevent null ID
	*/
	class USkeletalMesh* GetHairByID(uint8 ID);

	/**
	*Returns a skeletal mesh based on the ID recieved
	*@param Could return NULL (if casts fail) but shouldnt
	*@param if ID is bigger than the vector ID returns the last item in the list to prevent null ID
	*/
	class USkeletalMesh* GetFaceByID(uint8 ID);

	/**
	*Returns a skeletal mesh based on the ID recieved
	*@param Could return NULL (if casts fail) but shouldnt
	*@param if ID is bigger than the vector ID returns the last item in the list to prevent null ID
	*/
	class USkeletalMesh* GetShouldersByID(uint8 ID);

	/**
	*Returns a skeletal mesh based on the ID recieved
	*@param Could return NULL (if casts fail) but shouldnt
	*@param if ID is bigger than the vector ID returns the last item in the list to prevent null ID
	*/
	class USkeletalMesh* GetBodyByID(uint8 ID);

	/**
	*Returns a skeletal mesh based on the ID recieved
	*@param Could return NULL (if casts fail) but shouldnt
	*@param if ID is bigger than the vector ID returns the last item in the list to prevent null ID
	*/
	class USkeletalMesh* GetGlovesByID(uint8 ID);

	/**
	*Returns a skeletal mesh based on the ID recieved
	*@param Could return NULL (if casts fail) but shouldnt
	*@param if ID is bigger than the vector ID returns the last item in the list to prevent null ID
	*/
	class USkeletalMesh* GetBeltByID(uint8 ID);

	/**
	*Returns a skeletal mesh based on the ID recieved
	*@param Could return NULL (if casts fail) but shouldnt
	*@param if ID is bigger than the vector ID returns the last item in the list to prevent null ID
	*/
	class USkeletalMesh* GetShoesByID(uint8 ID);
};

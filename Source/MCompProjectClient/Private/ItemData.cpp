// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"
#include "Items/ItemBase.h"
#include "Engine/SkeletalMesh.h"

UItemData::UItemData()
{

}

USkeletalMesh* UItemData::GetHelmetByID(uint8 ID)
{
	if (ID >= _HeadItems.Num())
	{	
		return _HeadItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;
	}

	return _HeadItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;

	return NULL;
}

USkeletalMesh* UItemData::GetHairByID(uint8 ID)
{
	if (ID >= _HairItems.Num())
	{
		return _HairItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;
	}

	return _HairItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;

	return NULL;
}

USkeletalMesh* UItemData::GetFaceByID(uint8 ID)
{
	if (ID >= _FaceItems.Num())
	{
		return _FaceItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;
	}

	return _FaceItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;

	return NULL;
}

USkeletalMesh* UItemData::GetShouldersByID(uint8 ID)
{
	if (ID >= _ShouldersItems.Num())
	{
		return _ShouldersItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;
	}

	return _ShouldersItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;

	return NULL;
}

USkeletalMesh* UItemData::GetBodyByID(uint8 ID)
{
	if (ID >= _BodyItems.Num())
	{
		return _BodyItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;
	}

	return _BodyItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;

	return NULL;
}

USkeletalMesh* UItemData::GetGlovesByID(uint8 ID)
{
	if (ID >= _GlovesItems.Num())
	{
		return _GlovesItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;
	}

	return _GlovesItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;

	return NULL;
}

USkeletalMesh* UItemData::GetBeltByID(uint8 ID)
{
	if (ID >= _BeltItems.Num())
	{
		return _BeltItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;
	}

	return _BeltItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;

	return NULL;
}

USkeletalMesh* UItemData::GetShoesByID(uint8 ID)
{
	if (ID >= _ShoesItems.Num())
	{
		return _ShoesItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;
	}

	return _ShoesItems[ID]->GetDefaultObject<UItemBase>()->_ItemMesh;

	return NULL;
}

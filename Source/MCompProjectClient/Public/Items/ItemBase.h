// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS(ABSTRACT, BLUEPRINTABLE)
class MCOMPPROJECTCLIENT_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UItemBase();

	UPROPERTY(EditAnywhere)
	class USkeletalMesh* _ItemMesh;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SavedPlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API USavedPlayerInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	USavedPlayerInfo();

	UPROPERTY(EditAnywhere)
	FString _PlayerName{ "No Username Given" };
};

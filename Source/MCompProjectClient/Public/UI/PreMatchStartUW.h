// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "PreMatchStartUW.generated.h"

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API UPreMatchStartUW : public UUserWidget
{
	GENERATED_BODY()

private:
	class UTextBox* _Text;

	
};

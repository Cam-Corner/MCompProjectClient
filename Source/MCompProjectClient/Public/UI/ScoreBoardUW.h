// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "ScoreBoardUW.generated.h"

USTRUCT() struct FScoreboardKD
{
	GENERATED_BODY()

	FString Username = "";
	int32 Kills = 0;
	int32 Deaths = 0;
};

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API UScoreBoardUW : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void UpdateScoreBoard();

protected:
	UPROPERTY(meta = (BindWidget))
		UVerticalBox* _PlayerNamesVB;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* _PlayerKillsVB;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* _PlayerDeathsVB;

private:
	void InsertionSortArray(TArray<FScoreboardKD>& Array);
};

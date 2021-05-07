// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ScoreBoardUW.h"
#include "Networking/MPGameState.h"
#include "Networking/MPPlayerState.h"
#include "Kismet/GameplayStatics.h"


void UScoreBoardUW::NativeConstruct()
{
	
}

void UScoreBoardUW::UpdateScoreBoard()
{
	//Get all the players and their KD's
	TArray<FScoreboardKD> PlayersKD;
	if (AMPGameState* GS = Cast<AMPGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		for (int i = 0; i < GS->PlayerArray.Num(); i++)
		{
			if (AMPPlayerState* PS = Cast<AMPPlayerState>(GS->PlayerArray[i]))
			{
				FScoreboardKD TempKD;
				PS->GetNameAndKD(TempKD.Username, TempKD.Kills, TempKD.Deaths);
				PlayersKD.Add(TempKD);
				UE_LOG(LogTemp, Warning, TEXT("This Players Kills: %s"), *FString::SanitizeFloat(TempKD.Kills));
			}
		}
	}

	//Order the KD list by who has the most kills
	InsertionSortArray(PlayersKD);

	TArray<UWidget*> UsernameWidgets = _PlayerNamesVB->GetAllChildren();
	TArray<UWidget*> PlayerKillsWidgets = _PlayerKillsVB->GetAllChildren();
	TArray<UWidget*> PlayerDeathsWidgets = _PlayerDeathsVB->GetAllChildren();
	for (int i = 0; i < UsernameWidgets.Num(); i++)
	{
		if (i < PlayersKD.Num())
		{
			if (UTextBlock* T = Cast<UTextBlock>(UsernameWidgets[i]))
			{
				T->SetText(FText::FromString(PlayersKD[i].Username));
				T->SetVisibility(ESlateVisibility::Visible);
			}

			if (i < PlayerKillsWidgets.Num())
			{
				if (UTextBlock* T = Cast<UTextBlock>(PlayerKillsWidgets[i]))
				{
					T->SetText(FText::FromString(FString::FromInt(PlayersKD[i].Kills)));
					T->SetVisibility(ESlateVisibility::Visible);
				}
			}

			if (i < PlayerDeathsWidgets.Num())
			{
				if (UTextBlock* T = Cast<UTextBlock>(PlayerDeathsWidgets[i]))
				{
					T->SetText(FText::FromString(FString::FromInt(PlayersKD[i].Deaths)));
					T->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
		else
		{
			if (UTextBlock* T = Cast<UTextBlock>(UsernameWidgets[i]))
			{
				T->SetVisibility(ESlateVisibility::Hidden);
			}

			if (i < PlayerKillsWidgets.Num())
			{
				if (UTextBlock* T = Cast<UTextBlock>(PlayerKillsWidgets[i]))
				{
					T->SetVisibility(ESlateVisibility::Hidden);
				}
			}

			if (i < PlayerDeathsWidgets.Num())
			{
				if (UTextBlock* T = Cast<UTextBlock>(PlayerDeathsWidgets[i]))
				{
					T->SetVisibility(ESlateVisibility::Hidden);
				}
			}

		}
		
	}

	//UE_LOG(LogTemp, Warning, TEXT("PlayersKD: %s, Widgets: %s"), *FString::SanitizeFloat(PlayersKD.Num()),
	//		*FString::SanitizeFloat(UsernameWidgets.Num()))
}

void UScoreBoardUW::InsertionSortArray(TArray<FScoreboardKD>& Array)
{
	if (Array.Num() <= 1)
		return;

	FScoreboardKD CurrentKD;
	for (int i = 1; i < Array.Num(); i++)
	{
		CurrentKD = Array[i]; // the current array index we are checking
		
		int j = i - 1; //checking index
		while (j >= 0 && Array[j].Kills < CurrentKD.Kills) // check if kills are bigger than the checking index kills
		{
			Array[j + 1] = Array[j];
			j -= 1;
		}
		Array[j + 1] = CurrentKD;
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MapVoteScreenUW.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Networking/MPPlayerController.h"

void UMapVoteScreenUW::NativeConstruct()
{
	_NoVoteButton->OnPressed.AddDynamic(this, &UMapVoteScreenUW::NoMapVote);
	_ForrestMapVoteButton->OnPressed.AddDynamic(this, &UMapVoteScreenUW::ForrestMapVote);
	_ExampleMapVoteButton->OnPressed.AddDynamic(this, &UMapVoteScreenUW::ExampleMapVote);

	_NoVoteButton->SetBackgroundColor(FLinearColor::Green);
	_ForrestMapVoteButton->SetBackgroundColor(FLinearColor::White);
	_ExampleMapVoteButton->SetBackgroundColor(FLinearColor::White);
}

void UMapVoteScreenUW::SendMapVote(int8 MapVote)
{
	if (AMPPlayerController* MPPC = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		MPPC->Server_SendMapVote(MapVote);
	}
}

void UMapVoteScreenUW::NoMapVote()
{
	SendMapVote(0);
	_NoVoteButton->SetBackgroundColor(FLinearColor::Green);
	_ForrestMapVoteButton->SetBackgroundColor(FLinearColor::White);
	_ExampleMapVoteButton->SetBackgroundColor(FLinearColor::White);
}

void UMapVoteScreenUW::ForrestMapVote()
{
	SendMapVote(1);

	_NoVoteButton->SetBackgroundColor(FLinearColor::White);
	_ForrestMapVoteButton->SetBackgroundColor(FLinearColor::Green);
	_ExampleMapVoteButton->SetBackgroundColor(FLinearColor::White);
}

void UMapVoteScreenUW::ExampleMapVote()
{
	SendMapVote(2);

	_NoVoteButton->SetBackgroundColor(FLinearColor::White);
	_ForrestMapVoteButton->SetBackgroundColor(FLinearColor::White);
	_ExampleMapVoteButton->SetBackgroundColor(FLinearColor::Green);
}

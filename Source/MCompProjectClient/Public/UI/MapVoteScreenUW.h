// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapVoteScreenUW.generated.h"

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API UMapVoteScreenUW : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(Meta = (BindWidget))
		class UButton* _NoVoteButton;

	UFUNCTION()
		void NoMapVote();

	UPROPERTY(Meta = (BindWidget))
		class UButton* _ForrestMapVoteButton;

	UFUNCTION()
		void ForrestMapVote();

	UPROPERTY(Meta = (BindWidget))
		class UButton* _ExampleMapVoteButton;

	UFUNCTION()
		void ExampleMapVote();

protected:
	void SendMapVote(int8 MapVote);
};

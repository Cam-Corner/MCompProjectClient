// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MPGameState.generated.h"

/*USTRUCT(Blueprintable)
struct FChatMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString _PlayerWhoSentMessage { "Unknown" };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString _Message { "Nothing" };
};*/


/**
 * 
 */

struct FGameTimer;

UCLASS()
class MCOMPPROJECTCLIENT_API AMPGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AMPGameState(const FObjectInitializer& ObjectInitializer);

	void AddScore(uint8 PlayerNumber);

	void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		FString GetTimeRemaining();

	void Restart();
	/*UFUNCTION(Server, UnReliable, WithValidation, BlueprintCallable)
		void SendChatMessageToServer(const FChatMessage Message);

	UFUNCTION(BlueprintCallable)
		TArray<FChatMessage> GetChatMessages() { return _ClientChatMessages; }*/

protected:

	UPROPERTY(Replicated)
		int32 _PlayerOneScore;

	UPROPERTY(Replicated)
		int32 _PlayerTwoScore;

	UPROPERTY(Replicated)
		int32 _SomeNumber;

	void IncreaseVariable();

	/*
	* Replicated Seconds left of the match
	* only replicates the whole time as seconds
	* Client should convert seconds into seconds/minutes/hours timer that can be shown as UI
	*/
	UPROPERTY(Replicated)
		float _TimerRemaining{ 120 };


	/** Store New Client Message */
	//void StoreNewChatMessage(const FChatMessage Message);

private:
	/** Stores all the messages the clients have sent to each other*/
	/*TArray<FChatMessage> _ClientChatMessages;

	int32 _MaxStoredChatMessages = 15;*/
};

USTRUCT(BlueprintType)
struct FGameTimer
{
	GENERATED_USTRUCT_BODY()

public:
	FGameTimer() : _Seconds(0), _Minutes(0) {}

	float _Seconds = 0;
	uint32 _Minutes = 0;
	//uint32 _Hours = 0;


	FGameTimer ConvertSecondsToTime(float Seconds)
	{
		float SecondsLeft = (int)FMath::Fmod(Seconds, 60);
		float Minutes = (Seconds - SecondsLeft) / 60;

		FGameTimer TempTimer = FGameTimer();
		TempTimer._Seconds = SecondsLeft;
		TempTimer._Minutes = Minutes;
		return TempTimer;
	}


//static functions
public:
	static FGameTimer ConvertSecondsToTime_Static(float Seconds)
	{
		float SecondsLeft = (int)FMath::Fmod(Seconds, 60);
		float Minutes = (Seconds - SecondsLeft) / 60;

		FGameTimer TempTimer = FGameTimer();
		TempTimer._Seconds = SecondsLeft;
		TempTimer._Minutes = Minutes;
		return TempTimer;
	}

	static FString ConvertTimeForUI_Static(float Seconds)
	{
		return ConvertTimeForUI_Static(FGameTimer::ConvertSecondsToTime_Static(Seconds));
	}

	static FString ConvertTimeForUI_Static(FGameTimer Time)
	{
		FString TimeString = "";
		FString SecondsString = "";

		if (Time._Seconds < 10)
			SecondsString = "0" + FString::FromInt(Time._Seconds);
		else
			SecondsString = FString::FromInt(Time._Seconds);

		TimeString = FString::FromInt(Time._Minutes) + ":" + SecondsString;

		return TimeString;
	}
};
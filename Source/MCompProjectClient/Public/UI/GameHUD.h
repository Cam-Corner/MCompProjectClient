// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAGameHUD, Log, All);  

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	AGameHUD();

	virtual void BeginPlay() override;

	UFUNCTION()
		void EscapePressed();

	/*
	* Add a new chat message to the ChatSystem Widget
	*/
	void AddNewChatMessage(FString DisplayName, FString Message);

	/*
	* Enable/disable chat system
	*/
	void EnableChatSystem(bool bEnable);

	/*
	* Enable/disable PreMatch Start UserWidget
	*/
	void EnablePreMatchStartUW(bool bEnable);

	/*
	* Enable/disable PreMatch Start UserWidget
	*/
	void EnablePlayerUIUW(bool bEnable);

	/*
	* Set the health of the health bar
	*/
	void SetHealthBar(const float CurrentHealth, const float PlayersMaxHealth);

	/*
	* Set the Timer by giving it the timer by string
	*/
	void SetTimeLeft(const FString TimerLeftText);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Widgets")
	TAssetSubclassOf<class UChatSystemUI> _ChatSystemUW;

	UPROPERTY(EditDefaultsOnly, Category = "Game Widgets")
	TAssetSubclassOf<class UUserWidget> _PreMatchStartUW;

	UPROPERTY(EditDefaultsOnly, Category = "Game Widgets")
	TAssetSubclassOf<class UPlayerUI> _PlayerUIUW;

private:
	//Chat System
	class UChatSystemUI* _ActiveChatSystemUW;

	class UPlayerUI* _ActivePlayerUI;

	void CreateChatSystemWidget();

	//PreMatch Start Widget
	class UUserWidget* _ActivePreMatchStartUW;

	void CreatePreMatchStartWidget();

	void CreatePlayerUIWidget();
};

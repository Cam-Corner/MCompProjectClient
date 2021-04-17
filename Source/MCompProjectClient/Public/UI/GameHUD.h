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
	* Enable/disable chat system
	*/
	void EnableChatSystem(bool bEnable);

	/*
	* Enable/disable PreMatch Start UserWidget
	*/
	void EnablePreMatchStartUW(bool bEnable);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Widgets")
	TAssetSubclassOf<class UChatSystemUI> _ChatSystemUW;

	UPROPERTY(EditDefaultsOnly, Category = "Game Widgets")
	TAssetSubclassOf<class UUserWidget> _PreMatchStartUW;

private:
	//Chat System
	class UChatSystemUI* _ActiveChatSystemUW;

	void CreateChatSystemWidget();

	//PreMatch Start Widget
	class UUserWidget* _ActivePreMatchStartUW;

	void CreatePreMatchStartWidget();
};

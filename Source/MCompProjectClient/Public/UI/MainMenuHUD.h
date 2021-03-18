// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

enum EMenuTypes
{
	EMT_MainMenu,
	EMT_CustomizationTab,
	EMT_SettingsTab,
};

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	/** Shows the UI HUD */
	UFUNCTION(BlueprintCallable)
	void ShowMainMenu();

	/** Removes the UI HUD from the screen */
	UFUNCTION(BlueprintCallable)
	void RemoveMainMenu();

	void JoinServer();

	void ChangeUITabTo(const EMenuTypes Menu);

protected:
	TSharedPtr<class SMainMenuWidget> _MainMenuWidget;
	TSharedPtr<class SWidget> _MainMenuContainer;

	TSharedPtr<class SCustomizeCharacterWidget> _CustomisationWidget;
	TSharedPtr<class SWidget> _CustomisationContainer;

	EMenuTypes _CurrentMenu = EMenuTypes::EMT_MainMenu;

	void ShowCustomisationUI();
	void RemoveCustomisationUI();
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuHUD.h"
#include "UI/MainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SCustomizeCharacterWidget.h"
#include "Other/CharacterCustomiserActor.h"


void AMainMenuHUD::ShowMainMenu()
{
	if (GEngine && GEngine->GameViewport)
	{
		_MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(_MainMenuContainer, SWeakWidget)
			.PossiblyNullContent(_MainMenuWidget.ToSharedRef()));

		

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->bEnableClickEvents = true;
			PlayerOwner->bEnableMouseOverEvents = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
}

void AMainMenuHUD::RemoveMainMenu()
{
	GEngine->GameViewport->RemoveViewportWidgetContent(_MainMenuContainer.ToSharedRef());

	//if (PlayerOwner)
	//{
	//	PlayerOwner->bShowMouseCursor = false;
	//	PlayerOwner->SetInputMode(FInputModeGameOnly());
	//}
}

void AMainMenuHUD::JoinServer()
{
	UGameplayStatics::OpenLevel(GetWorld(), "127.0.0.1", true, "?game = AMPGameMode");
}

void AMainMenuHUD::ChangeUITabTo(const EMenuTypes Menu)
{
	switch(_CurrentMenu)
	{
	case EMT_MainMenu:
		RemoveMainMenu();
		break;
	case EMT_CustomizationTab:
		RemoveCustomisationUI();
		break;
	default:
		break;
	}

	switch (Menu)
	{
	case EMT_MainMenu:
		ShowMainMenu();
		break;
	case EMT_CustomizationTab:
		ShowCustomisationUI();
		break;
	default:
		break;
	}

	_CurrentMenu = Menu;
}

void AMainMenuHUD::ShowCustomisationUI()
{
	if (GEngine && GEngine->GameViewport)
	{
		ACharacterCustomiserActor* CCA = Cast<ACharacterCustomiserActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterCustomiserActor::StaticClass()));

		_CustomisationWidget = SNew(SCustomizeCharacterWidget).CCA(CCA).OwningHUD(this);
		GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(_CustomisationContainer, SWeakWidget)
			.PossiblyNullContent(_CustomisationWidget.ToSharedRef()));

		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->bEnableClickEvents = true;
			PlayerOwner->bEnableMouseOverEvents = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}

		
		if (AActor* CCActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterCustomiserActor::StaticClass()))
		{
			if (ACharacterCustomiserActor* CC = Cast<ACharacterCustomiserActor>(CCActor))
			{
				CC->SetVisibility(true);
			}
		}
	}
}

void AMainMenuHUD::RemoveCustomisationUI()
{
	GEngine->GameViewport->RemoveViewportWidgetContent(_CustomisationContainer.ToSharedRef());

	if (AActor* CCActor = UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterCustomiserActor::StaticClass()))
	{
		if (ACharacterCustomiserActor* CC = Cast<ACharacterCustomiserActor>(CCActor))
		{
			CC->SetVisibility(false);
		}
	}
}
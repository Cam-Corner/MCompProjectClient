// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuHUD.h"
#include "UI/MainMenuWidget.h"
#include "Widgets/SWeakWidget.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SCustomizeCharacterWidget.h"
#include "Other/CharacterCustomiserActor.h"
#include "UI/MainMenuUW.h"
#include "UObject/ConstructorHelpers.h"
#include "MPGameInstance.h"


DEFINE_LOG_CATEGORY(LogAMainMenuHUD);

AMainMenuHUD::AMainMenuHUD()
{
	static ConstructorHelpers::FClassFinder<UMainMenuUW> MainMenuUW(TEXT("/Game/UI/UW_MainMenu"));
	if (MainMenuUW.Succeeded())
	{
		_MainMenuUW = MainMenuUW.Class;
	}
	else
	{
		UE_LOG(LogAMainMenuHUD, Error, TEXT("Coud not find widget blueprint for MainMenuUW (Constructor Helpers)"));
	}
}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	//if (GetLocalRole() != ROLE_Authority)
	//{
		//CreateMainMenuUW();
		//EnableMainMenuUW(false);
	//}

}

void AMainMenuHUD::Tick(float DeltaTime)
{

}

void AMainMenuHUD::ShowMainMenu()
{
	//if (GEngine && GEngine->GameViewport)
	//{
	//	_MainMenuWidget = SNew(SMainMenuWidget).OwningHUD(this);
	//	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(_MainMenuContainer, SWeakWidget)
	//		.PossiblyNullContent(_MainMenuWidget.ToSharedRef()));
	//
	//	
	//
	//	if (PlayerOwner)
	//	{
	//		PlayerOwner->bShowMouseCursor = true;
	//		PlayerOwner->bEnableClickEvents = true;
	//		PlayerOwner->bEnableMouseOverEvents = true;
	//		PlayerOwner->SetInputMode(FInputModeUIOnly());
	//	}
	//}

	//ChangeUITabTo(EMenuTypes::EMT_MainMenu);
	CreateMainMenuUW();
	EnableMainMenuUW(true);

	if (UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		if (GI->WasPlayerKickedFromServer())
		{
			if (_ActiveMainMenuUW != NULL)
			{
				_ActiveMainMenuUW->SetKickReason(GI->GetKickReason());
			}
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

void AMainMenuHUD::JoinServerByIP(FName ServerIP)
{
	UGameplayStatics::OpenLevel(GetWorld(), ServerIP, true, "?game = AMPGameMode");
}

void AMainMenuHUD::ChangeUITabTo(const EMenuTypes Menu)
{
	switch(_CurrentMenu)
	{
	case EMT_MainMenu:
		EnableMainMenuUW(false);
		//RemoveMainMenu();
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
		EnableMainMenuUW(true);
		//ShowMainMenu();
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

void AMainMenuHUD::CreateMainMenuUW()
{
	if (_MainMenuUW == NULL || _ActiveMainMenuUW != NULL)
		return;

	_ActiveMainMenuUW = CreateWidget<UMainMenuUW>(GetOwningPlayerController(), _MainMenuUW.LoadSynchronous());
	if (_ActiveMainMenuUW == nullptr)
	{
		UE_LOG(LogAMainMenuHUD, Error, TEXT("Could not create widget: MainMenuUW"));
		return;
	}

	_ActiveMainMenuUW->AddToViewport();
	EnableMainMenuUW(false);
}

void AMainMenuHUD::EnableMainMenuUW(bool bEnable)
{
	if (_ActiveMainMenuUW == nullptr)
		return;

	if (bEnable)
		_ActiveMainMenuUW->SetVisibility(ESlateVisibility::Visible);
	else
		_ActiveMainMenuUW->SetVisibility(ESlateVisibility::Hidden);
}
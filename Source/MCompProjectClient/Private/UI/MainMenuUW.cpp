// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuUW.h"
#include "Components/Button.h"
#include "UI/MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Networking/MPPlayerController.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/RichTextBlock.h"
#include "Components/MultiLineEditableText.h"

void UMainMenuUW::NativeConstruct()
{
	_PlayButton->OnPressed.AddDynamic(this, &UMainMenuUW::PlayButtonPressed);
	_CustomizationButton->OnPressed.AddDynamic(this, &UMainMenuUW::CustomizationButtonPressed);
	_RemoveKickMessageButton->OnPressed.AddDynamic(this, &UMainMenuUW::RemoveKickReason);
	_QuitButton->OnPressed.AddDynamic(this, &UMainMenuUW::QuitButtonPressed);
	_ConnectIP->OnPressed.AddDynamic(this, &UMainMenuUW::ConnectToServerByIP);

	if (APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (AMainMenuHUD* MainMenuHUD = Cast<AMainMenuHUD>(PC->GetHUD()))
		{
			_MainMenuHUD = MainMenuHUD;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Could not cast to AMainMenuHUD"));
		}
	}

	RemoveKickReason();
}

void UMainMenuUW::ConnectToServerByIP()
{
	FName IP = (FName)_ConnectByIPText->GetText().ToString();
	_MainMenuHUD->JoinServerByIP(IP);
}

void UMainMenuUW::PlayButtonPressed()
{
	if (_MainMenuHUD == NULL)
		return;

	_MainMenuHUD->JoinServer();
}

void UMainMenuUW::CustomizationButtonPressed()
{
	if (_MainMenuHUD == NULL)
		return;

	_MainMenuHUD->ChangeUITabTo(EMenuTypes::EMT_CustomizationTab);
}

void UMainMenuUW::QuitButtonPressed()
{
	if (_MainMenuHUD == NULL)
		return;

	if (APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PC->ConsoleCommand("quit");
	}
}

void UMainMenuUW::SetKickReason(FString KickReason)
{
	_KickReasonText->SetText(FText::FromString(KickReason));
	ShowKickReasonUI(true);
}

void UMainMenuUW::RemoveKickReason()
{
	ShowKickReasonUI(false);
}

void UMainMenuUW::ShowKickReasonUI(bool bEnable)
{
	if (bEnable)
	{
		_KickReasonBackground->SetVisibility(ESlateVisibility::Visible);
		_KickReasonText->SetVisibility(ESlateVisibility::Visible);
		_RemoveKickMessageButton->SetVisibility(ESlateVisibility::Visible);

		if (APlayerController* PlayerOwner = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->bEnableClickEvents = true;
			PlayerOwner->bEnableMouseOverEvents = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
	else
	{
		_KickReasonBackground->SetVisibility(ESlateVisibility::Hidden);
		_KickReasonText->SetVisibility(ESlateVisibility::Hidden);
		_RemoveKickMessageButton->SetVisibility(ESlateVisibility::Hidden);

		//if (APlayerController* PlayerOwner = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		//{
		//	PlayerOwner->bShowMouseCursor = false;
		//	PlayerOwner->bEnableClickEvents = false;
		//	PlayerOwner->bEnableMouseOverEvents = false;
		//	//PlayerOwner->SetInputMode(FInputModeUIOnly());
		//}
	}
}

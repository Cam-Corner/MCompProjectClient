// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameHUD.h"
#include "Networking/ChatSystemUI.h"
#include "UI/PlayerUI.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY(LogAGameHUD);

AGameHUD::AGameHUD()
{
	//find the Chat System UserWidget Blueprint
	static ConstructorHelpers::FClassFinder<UChatSystemUI> ChatUW(TEXT("/Game/UI/ChatSystem/UW_ChatSystem"));
	if (ChatUW.Succeeded())
	{
		_ChatSystemUW = ChatUW.Class;
	}
	else
	{
		UE_LOG(LogAGameHUD, Error, TEXT("Could not find widget blueprint for ChatSystem (Constructor Helpers)"));
	}

	//find the PreMatch start UserWidget Blueprint
	static ConstructorHelpers::FClassFinder<UUserWidget> PreMatchUW(TEXT("/Game/UI/UW_PreMatchStart"));
	if (PreMatchUW.Succeeded())
	{
		_PreMatchStartUW = PreMatchUW.Class;
	}
	else
	{
		UE_LOG(LogAGameHUD, Error, TEXT("Coud not find widget bluerping for PreMatchStart (Constructor Helpers)"));
	}

	static ConstructorHelpers::FClassFinder<UPlayerUI> PlayerUW(TEXT("/Game/UI/UW_PlayerUI"));
	if (PlayerUW.Succeeded())
	{
		_PlayerUIUW = PlayerUW.Class;
	}
	else
	{
		UE_LOG(LogAGameHUD, Error, TEXT("Coud not find widget bluerping for PlayerUI (Constructor Helpers)"));
	}
}

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if (GEngine && GEngine->GameViewport)
	{
		CreateChatSystemWidget();
		CreatePreMatchStartWidget();
		CreatePlayerUIWidget();
	}
}

void AGameHUD::SetTimeLeft(const FString TimerLeftText)
{
	if (_ActivePlayerUI == NULL)
		return;

	_ActivePlayerUI->SetTimeLeft(TimerLeftText);
}

void AGameHUD::CreateChatSystemWidget()
{
	if (_ChatSystemUW == NULL || _ActiveChatSystemUW != NULL)
		return;

	_ActiveChatSystemUW = CreateWidget<UChatSystemUI>(GetOwningPlayerController(), _ChatSystemUW.LoadSynchronous());
	if (_ActiveChatSystemUW == NULL)
	{
		UE_LOG(LogAGameHUD, Error, TEXT("Could not Create Widget: AChatSystemUI "));
		return;
	}

	_ActiveChatSystemUW->AddToViewport();
	EnableChatSystem(false);

	//for (int i = 0; i < 20; i++)
	//{
	//	_ActiveChatSystemUW->AddNewChatMessage(FString("Turnips"), FString("Message" + FString::SanitizeFloat(i)));
	//}
}

void AGameHUD::CreatePlayerUIWidget()
{
	if (_PlayerUIUW == NULL || _ActivePlayerUI != NULL)
		return;

	_ActivePlayerUI = CreateWidget<UPlayerUI>(GetOwningPlayerController(), _PlayerUIUW.LoadSynchronous());
	if (_ActivePlayerUI == nullptr)
	{
		UE_LOG(LogAGameHUD, Error, TEXT("Could not create widget: UPlayerUI"));
		return;
	}

	_ActivePlayerUI->AddToViewport();
	EnablePlayerUIUW(true);
}

void AGameHUD::CreatePreMatchStartWidget()
{
	if (_PreMatchStartUW == NULL || _ActivePreMatchStartUW != NULL)
		return;

	_ActivePreMatchStartUW = CreateWidget<UUserWidget>(GetOwningPlayerController(), _PreMatchStartUW.LoadSynchronous());
	if (_ActivePreMatchStartUW == nullptr)
	{
		UE_LOG(LogAGameHUD, Error, TEXT("Could not create widget: APreMatchStartUW"));
		return;
	}

	_ActivePreMatchStartUW->AddToViewport();
	EnablePreMatchStartUW(false);
}

void AGameHUD::EscapePressed()
{
	UE_LOG(LogAGameHUD, Warning, TEXT("Enter Pressed"));
	EnableChatSystem(false);
}

void AGameHUD::EnablePreMatchStartUW(bool bEnable)
{
	if (_ActivePreMatchStartUW == nullptr)
		return;

	if (bEnable)
		_ActivePreMatchStartUW->SetVisibility(ESlateVisibility::Visible);
	else
		_ActivePreMatchStartUW->SetVisibility(ESlateVisibility::Hidden);
}

void AGameHUD::EnableChatSystem(bool bEnable)
{
	if (_ActiveChatSystemUW == nullptr)
		return;

	//if (bEnable)
	//	_ActiveChatSystemUW->SetVisibility(ESlateVisibility::Visible);
	//else
	//	_ActiveChatSystemUW->SetVisibility(ESlateVisibility::Hidden);

	if (bEnable)
	{
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->bEnableClickEvents = true;
			PlayerOwner->bEnableMouseOverEvents = true;
			//PlayerOwner->SetInputMode(FInputModeUIOnly());
		}

		UE_LOG(LogAGameHUD, Warning, TEXT("Chat System Enabled"));
	}
	else
	{
		if (PlayerOwner)
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->bEnableClickEvents = false;
			PlayerOwner->bEnableMouseOverEvents = false;
			//PlayerOwner->SetInputMode(FInputModeGameOnly());
		}

		UE_LOG(LogAGameHUD, Warning, TEXT("Chat System Disabled"));
	}

	_ActiveChatSystemUW->EnableTyping(bEnable);
}

void AGameHUD::EnablePlayerUIUW(bool bEnable)
{
	if (_ActivePlayerUI == nullptr)
		return;

	if (bEnable)
		_ActivePlayerUI->SetVisibility(ESlateVisibility::Visible);
	else
		_ActivePlayerUI->SetVisibility(ESlateVisibility::Hidden);
}

void AGameHUD::SetHealthBar(const float CurrentHealth, const float PlayersMaxHealth)
{
	if (_ActivePlayerUI == nullptr)
		return;

	float CurrentHealthPercent = (CurrentHealth / (PlayersMaxHealth / 100)) / 100;

	_ActivePlayerUI->SetHealth(CurrentHealthPercent);
}

void AGameHUD::AddNewChatMessage(FString DisplayName, FString Message)
{
	if (_ActiveChatSystemUW == NULL)
		return;

	_ActiveChatSystemUW->AddNewChatMessage(DisplayName, Message);
}

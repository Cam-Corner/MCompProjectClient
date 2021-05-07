// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/ChatSystemUI.h"
#include "Networking/MPPlayerState.h"
#include "Networking/MPPlayerController.h"
#include "Kismet/GameplayStatics.h"


void UChatSystemUI::NativeConstruct()
{
	for (int i = 0; i < 20; i++)
	{
		_Messages.Add(" ");
		
	}


	//GEngine->AddOnScreenDebugMessage(9, 20.0f, FColor::Black, FString::SanitizeFloat(_Messages.Num()));
	{
		_MessageBoxes.Add(_Message00);
		_MessageBoxes.Add(_Message01);
		_MessageBoxes.Add(_Message02);
		_MessageBoxes.Add(_Message03);
		_MessageBoxes.Add(_Message04);
		_MessageBoxes.Add(_Message05);
		_MessageBoxes.Add(_Message06);
		_MessageBoxes.Add(_Message07);
		_MessageBoxes.Add(_Message08);
		_MessageBoxes.Add(_Message09);
		_MessageBoxes.Add(_Message10);
		_MessageBoxes.Add(_Message11);
		_MessageBoxes.Add(_Message12);
		_MessageBoxes.Add(_Message13);
		_MessageBoxes.Add(_Message14);
		_MessageBoxes.Add(_Message15);
		_MessageBoxes.Add(_Message16);
		_MessageBoxes.Add(_Message17);
		_MessageBoxes.Add(_Message18);
		_MessageBoxes.Add(_Message19);	
	}

	_ChatMessagesScrollBox->ScrollToEnd();
	_TypingTextBox->OnTextCommitted.AddDynamic(this, &UChatSystemUI::CommitChatMessage);	
}

void UChatSystemUI::AddNewChatMessage(FString DisplayName, FString Message)
{

	FString NewMessage = FString("<UserName>") + DisplayName + ": " +
						 FString("</><ChatMessage>")  + Message + FString("</>");

	//GEngine->AddOnScreenDebugMessage(0, 20.0f, FColor::Black, NewMessage);

	for (int i = 0; i < _Messages.Num(); i++)
	{
		if (i < _Messages.Num() - 1)
		{
			_Messages[i] = _Messages[i + 1];
		}
	}

	_Messages[_Messages.Num() - 1] = NewMessage;

	UpdateChatMessagesUI();

	_ChatMessagesScrollBox->ScrollToEnd();
}

void UChatSystemUI::CommitChatMessage(const FText& Text, ETextCommit::Type CommitMethod)
{
	FSlateApplication::Get().SetUserFocusToGameViewport(0);

	//if (CommitMethod != ETextCommit::OnEnter)
	//	return;

	_TypingTextBox->SetText(FText::FromString("Press 'T' to start typing..."));

	if (AMPPlayerController* PC = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PC->bShowMouseCursor = false;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
		PC->SetInputMode(FInputModeGameOnly());
	}

	if (CommitMethod != ETextCommit::OnEnter)
	{
		_TypingTextBox->SetText(FText::FromString("Press 'T' to start typing..."));
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
		return;
	}

	SendChatMessage();
}

FReply UChatSystemUI::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{

	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		SendChatMessage();
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		if (AMPPlayerController* PC = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			PC->bShowMouseCursor = false;
			PC->bEnableClickEvents = false;
			PC->bEnableMouseOverEvents = false;
			PC->SetInputMode(FInputModeGameOnly());
		}

		_TypingTextBox->SetText(FText::FromString("Press 'T' to start typing..."));
		FSlateApplication::Get().SetUserFocusToGameViewport(0);
	}

	return FReply::Unhandled();
}

void UChatSystemUI::SendChatMessage()
{
	if (AMPPlayerController* PC = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PC->bShowMouseCursor = false;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
		PC->SetInputMode(FInputModeGameOnly());

		if (AMPPlayerState* PS = Cast<AMPPlayerState>(PC->PlayerState))
		{
			FChatMessage Message;
			Message._ChatMessage = _TypingTextBox->GetText().ToString();
			Message._ClientsUsername = PS->GetPlayerName();
			PS->Server_SendChatMessage(Message);
		}
	}

	_TypingTextBox->SetText(FText::FromString("Press 'T' to start typing..."));
	FSlateApplication::Get().SetUserFocusToGameViewport(0);
}

void UChatSystemUI::UpdateChatMessagesUI()
{
	for (int i = 0; i < _MessageBoxes.Num(); i++)
	{
		_MessageBoxes[i]->SetText(FText::FromString(_Messages[i]));
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "UpdatedChatUI");
}

void UChatSystemUI::EnableTyping(bool bEnable)
{
	if (bEnable)
	{
		_TypingTextBox->SetVisibility(ESlateVisibility::Visible);
		//_ChatMessagesScrollBox->SetFocus();
		_TypingTextBox->SetKeyboardFocus();
		//FSlateApplication::Get().SetKeyboardFocus(_TypingTextBox, EFocusCause::SetDirectly);
		
		if (AMPPlayerController* PC = Cast<AMPPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = true;
			PC->bEnableMouseOverEvents = true;
			FInputModeUIOnly NewInputMode;

			//NewInputMode.SetWidgetToFocus(_CanvasPanel);
			PC->SetInputMode(NewInputMode);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Typing Enabled"));
		_TypingTextBox->SetText(FText::FromString(""));
	}
	else
	{
		_TypingTextBox->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("Typing Disabled"));		
	}
}
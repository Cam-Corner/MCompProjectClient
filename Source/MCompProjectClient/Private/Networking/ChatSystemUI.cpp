// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/ChatSystemUI.h"

void UChatSystemUI::NativeConstruct()
{
	for (int i = 0; i < 20; i++)
	{
		_Messages.Add(" ");
		
	}
	GEngine->AddOnScreenDebugMessage(9, 20.0f, FColor::Black, FString::SanitizeFloat(_Messages.Num()));
	{
		_MessageBoxes.Add(_Message19);
		_MessageBoxes.Add(_Message18);
		_MessageBoxes.Add(_Message17);
		_MessageBoxes.Add(_Message16);
		_MessageBoxes.Add(_Message15);
		_MessageBoxes.Add(_Message14);
		_MessageBoxes.Add(_Message13);
		_MessageBoxes.Add(_Message12);
		_MessageBoxes.Add(_Message11);
		_MessageBoxes.Add(_Message10);
		_MessageBoxes.Add(_Message09);
		_MessageBoxes.Add(_Message08);
		_MessageBoxes.Add(_Message07);
		_MessageBoxes.Add(_Message06);
		_MessageBoxes.Add(_Message05);
		_MessageBoxes.Add(_Message04);
		_MessageBoxes.Add(_Message03);
		_MessageBoxes.Add(_Message02);
		_MessageBoxes.Add(_Message01);
		_MessageBoxes.Add(_Message00);
	}
}

void UChatSystemUI::AddNewChatMessage(FString DisplayName, FString Message)
{

	FString NewMessage = DisplayName + ": " + Message;

	GEngine->AddOnScreenDebugMessage(0, 20.0f, FColor::Black, NewMessage);

	for (int i = 0; i < _Messages.Num(); i++)
	{
		if (i < _Messages.Num() - 1)
		{
			_Messages[i] = _Messages[i + 1];
		}
	}

	_Messages[_Messages.Num() - 1] = NewMessage;

	UpdateChatMessagesUI();
}

void UChatSystemUI::UpdateChatMessagesUI()
{
	for (int i = 0; i < _MessageBoxes.Num(); i++)
	{
		if (i < _Messages.Num())
		{
			_MessageBoxes[i]->SetText(FText::FromString(_Messages[i]));
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Black, "UpdatedChatUI");
}

void UChatSystemUI::EnableTyping(bool bEnable)
{
	if (bEnable)
	{
		_TypingTextBox->SetVisibility(ESlateVisibility::Visible);
		_TypingTextBox->SetKeyboardFocus();
		
		UE_LOG(LogTemp, Warning, TEXT("Typing Enabled"));
	}
	else
	{
		_TypingTextBox->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Warning, TEXT("Typing Disabled"));
		
	}
}
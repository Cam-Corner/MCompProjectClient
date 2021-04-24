// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/MultiLineEditableText.h"
#include "ChatSystemUI.generated.h"


/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API UChatSystemUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void AddNewChatMessage(FString DisplayName, FString Message);

	void EnableTypeBox(bool bEnable);

	/*
	* Enable typing
	* @Will listen to keyboard and start typing in the chatbox
	*/
	void EnableTyping(bool bEnable);

private:
	TArray<FString> _Messages;
	TArray<UTextBlock*> _MessageBoxes;

	void UpdateChatMessagesUI();

	UFUNCTION()
	void CommitChatMessage(const FText& Text, ETextCommit::Type CommitMethod);


	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	void SendChatMessage();

private:	
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableText* _TypingTextBox;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* _Message00;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message01;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message02;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message03;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message04;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message05;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message06;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message07;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message08;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message09;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message10;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message11;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message12;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message13;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message14;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message15;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message16;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message17;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message18;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* _Message19;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/MultiLineEditableText.h"
#include "Components/ScrollBox.h"
#include "Components/CanvasPanel.h"
#include "Components/RichTextBlock.h"
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
	TArray<URichTextBlock*> _MessageBoxes;

	void UpdateChatMessagesUI();

	UFUNCTION()
	void CommitChatMessage(const FText& Text, ETextCommit::Type CommitMethod);


	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	void SendChatMessage();

private:	
	UPROPERTY(meta = (BindWidget))
	UMultiLineEditableText* _TypingTextBox;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* _CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* _ChatMessagesScrollBox;

	UPROPERTY(meta = (BindWidget))
	URichTextBlock* _Message00;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message01;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message02;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message03;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message04;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message05;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message06;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message07;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message08;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message09;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message10;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message11;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message12;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message13;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message14;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message15;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message16;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message17;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message18;

	UPROPERTY(meta = (BindWidget))
		URichTextBlock* _Message19;

};

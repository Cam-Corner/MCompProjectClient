// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainMenuWidget.h"
#include "UI/MainMenuHUD.h"
#include "Components/Button.h"

#define LOCTEXT_NAMESPACE "MainMenu"

void SMainMenuWidget::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	OwningHUD = InArgs._OwningHUD;

	const FMargin ContentPadding = FMargin(500.0f, 300.0f);
	const FMargin ButtonPadding = FMargin(10, 10.0f);
	//const FMargin ButtonPadding = FMargin(10.0f);

	const FVector2D ButtonSize = FVector2D(1.0f, 1.0f);
	const FText TitleText = LOCTEXT("Game Title", "Main Menu");
	const FText PlayGameText = LOCTEXT("PlayerGame", "Play");
	const FText CustomiseGameText = LOCTEXT("CustomiseCharacter", "Customise Character");
	const FText QuitGameText = LOCTEXT("QuitGame", "Quit");

	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.0f;
	FSlateFontInfo TitleTextStyle = ButtonTextStyle;
	TitleTextStyle.Size = 60.0f;

	ChildSlot
		[
			SNew(SOverlay)
			//+ SOverlay::Slot()
			//.HAlign(HAlign_Fill)
			//.VAlign(VAlign_Fill)
			//[
			//	SNew(SImage)
			//	.ColorAndOpacity(FColor::Red)
			//]
			+SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			//.Padding(ContentPadding)
			[
				SNew(SVerticalBox) 
				
				//TitleText
				+ SVerticalBox::Slot()
				[
					SNew(STextBlock)
					.Font(TitleTextStyle)
					.Text(TitleText)
					.Justification(ETextJustify::Center)
				]
			]
			
			//Button slot
			+SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(ButtonPadding)
			[

				SNew(SVerticalBox)
				
				//Play Button
				+SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.DesiredSizeScale(ButtonSize)
					.OnClicked(this, &SMainMenuWidget::OnPlayClicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(PlayGameText)
						.Justification(ETextJustify::Left)
					]
				]

				//Customize character button
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.DesiredSizeScale(ButtonSize)
					.OnClicked(this, &SMainMenuWidget::OnCustomizeClicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(CustomiseGameText)
						.Justification(ETextJustify::Left)
					]
				]

				//Quit Button
				+ SVerticalBox::Slot()
				.Padding(ButtonPadding)
				[
					SNew(SButton)
					.DesiredSizeScale(ButtonSize)
					.OnClicked(this, &SMainMenuWidget::OnQuitClicked)
					[
						SNew(STextBlock)
						.Font(ButtonTextStyle)
						.Text(QuitGameText)
						.Justification(ETextJustify::Left)
					]
				]
			]
		];
}

FReply SMainMenuWidget::OnPlayClicked() const
{
	if (OwningHUD.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Button Clicked!"));
		OwningHUD->JoinServer();
	}

	return FReply::Handled();
}

FReply SMainMenuWidget::OnCustomizeClicked() const
{
	if (OwningHUD.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Button Clicked!"));
		OwningHUD->ChangeUITabTo(EMenuTypes::EMT_CustomizationTab);
	}

	return FReply::Handled();
}

FReply SMainMenuWidget::OnQuitClicked() const
{
	if (OwningHUD.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Button Clicked!"));
		if (APlayerController* PC = OwningHUD->PlayerOwner)
		{
			PC->ConsoleCommand("quit");
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE


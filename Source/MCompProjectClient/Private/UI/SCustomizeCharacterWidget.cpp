// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SCustomizeCharacterWidget.h"
#include "SlateOptMacros.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Widgets/Layout/SBox.h"
#include "Other/CharacterCustomiserActor.h"
#include "UI/MainMenuHUD.h"

#define LOCTEXT_NAMESPACE "MainMenu"
BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SCustomizeCharacterWidget::Construct(const FArguments& InArgs)
{
	const FVector2D GearButtonSize{ 0.5f, 0.5f };

	const float TextFillWidth = 250.0f;

	const FMargin ButtonPadding = FMargin(10.0f, 0.0f);
	const FMargin TextPadding = FMargin(200.0f, 500.0f);
	const FMargin OverlayPadding = FMargin(10.0f, 150.0f);
	const FMargin CustomSlotPadding = FMargin(100.0f, 100.0f);

	_CCA = InArgs._CCA;
	_OwningHUD = InArgs._OwningHUD;

	TArray<FText> _ButtonsNames;
	{
		const FText HelmetText = LOCTEXT("HelmetText", "Head");
		_ButtonsNames.Add(HelmetText);

		const FText HairText = LOCTEXT("HairText", "Hair");
		_ButtonsNames.Add(HairText);

		const FText FaceText = LOCTEXT("FaceText", "Face");
		_ButtonsNames.Add(FaceText);

		const FText ShouldersText = LOCTEXT("ShouldersText", "Shoulders");
		_ButtonsNames.Add(ShouldersText);

		const FText BodyText = LOCTEXT("BodyText", "Body");
		_ButtonsNames.Add(BodyText);

		const FText GlovesText = LOCTEXT("GlovesText", "Gloves");
		_ButtonsNames.Add(GlovesText);

		const FText BeltText = LOCTEXT("BeltText", "Belt");
		_ButtonsNames.Add(BeltText);

		const FText ShoesText = LOCTEXT("ShoesText", "Shoes");
		_ButtonsNames.Add(ShoesText);

	}

	TArray<FOnClicked> _ButtonIncrementFunctions;
	{
		_ButtonIncrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::IncreaseHeadGearInt));
		_ButtonIncrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::IncreaseHairTypeInt));
		_ButtonIncrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::IncreaseFaceTypeInt));
		_ButtonIncrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::IncreaseShouldersTypeInt));
		_ButtonIncrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::IncreaseBodyTypeInt));
		_ButtonIncrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::IncreaseGlovesTypeInt));
		_ButtonIncrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::IncreaseBeltTypeInt));
		_ButtonIncrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::IncreaseShoesTypeInt));
	}

	TArray<FOnClicked> _ButtonDecrementFunctions;
	{
		_ButtonDecrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::DecreaseHeadGearInt));
		_ButtonDecrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::DecreaseHairTypeInt));
		_ButtonDecrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::DecreaseFaceTypeInt));
		_ButtonDecrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::DecreaseShouldersTypeInt));
		_ButtonDecrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::DecreaseBodyTypeInt));
		_ButtonDecrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::DecreaseGlovesTypeInt));
		_ButtonDecrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::DecreaseBeltTypeInt));
		_ButtonDecrementFunctions.Add(FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::DecreaseShoesTypeInt));
	}

	FSlateFontInfo ButtonTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	ButtonTextStyle.Size = 40.0f;


	FString f = ("Nothing");
	//FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::DecreaseButtonPressed, f);
	//FOnClicked::CreateSP(this, &SCustomizeCharacterWidget::IncreaseButtonPressed, f);

	TSharedPtr<SVerticalBox> Container = SNew(SVerticalBox);

	for (int32 i = 0; i < _ButtonsNames.Num(); i++)
	{
		if (_ButtonIncrementFunctions.Num() > i && _ButtonDecrementFunctions.Num() > i)
		{
			Container->AddSlot()
				[
					SNew(SVerticalBox)
					//Helmet
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(FMargin(25.0f, 0.0f))
				[
					SNew(SHorizontalBox)

					//Decrease Head Gear Int
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(ButtonPadding)
					//.FillWidth(30)
					.AutoWidth()
					[
						SNew(SButton)
						.DesiredSizeScale(GearButtonSize)
						//.OnClicked(this, &SCustomizeCharacterWidget::DecreaseButtonPressed, FString("sadf"))
						.OnClicked(_ButtonDecrementFunctions[i])
					]

				//Text
				+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(FMargin(50.0f, 0.0f))
					//.AutoWidth()
					//.FillWidth(TextFillWidth)
					[
						SNew(STextBlock)
						.AutoWrapText(false)
					//.Clipping(EWidgetClipping::ClipToBoundsAlways)
					.Text(_ButtonsNames[i])
					.Font(ButtonTextStyle)
					.Justification(ETextJustify::Center)
					.ColorAndOpacity(FColor::Black)
					]


					//Increase Head Gear Int
					+SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					.Padding(FMargin(50.0f, 0.0f))
					//.FillWidth(30)
					.AutoWidth()
					[
						SNew(SButton)
						.DesiredSizeScale(GearButtonSize)
						.OnClicked(_ButtonIncrementFunctions[i])
					]
				]
			];
		}
	}

	//add the accept button to the container
	{
		Container->AddSlot()
			[
				SNew(SVerticalBox)
				//Helmet
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				//.Padding(FMargin(25.0f, 0.0f))
				[
					SNew(SHorizontalBox)

					//Decrease Head Gear Int
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Center)
					.VAlign(VAlign_Center)
					//.Padding(ButtonPadding)
					//.FillWidth(30)
					//.AutoWidth()
					[
						SNew(SButton)
						.DesiredSizeScale((50, 50))
						.OnClicked(this, &SCustomizeCharacterWidget::SaveItems)
					]
				]
			];
	}

	ChildSlot
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Center)
			.Padding(OverlayPadding)
			[
				Container.ToSharedRef()				
			]
		];
	
}

FReply SCustomizeCharacterWidget::SaveItems()
{
	if (_OwningHUD != NULL)
		_OwningHUD->ChangeUITabTo(EMenuTypes::EMT_MainMenu);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::IncreaseHeadGearInt()
{
	if (_CCA != NULL)
		_CCA->IncreaseArmourInt(1);

	
	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::DecreaseHeadGearInt()
{
	if (_CCA != NULL)
		_CCA->DecreaseArmourInt(1);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::IncreaseHairTypeInt()
{
	if (_CCA != NULL)
		_CCA->IncreaseArmourInt(2);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::DecreaseHairTypeInt()
{
	if (_CCA != NULL)
		_CCA->DecreaseArmourInt(2);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::IncreaseFaceTypeInt()
{
	if (_CCA != NULL)
		_CCA->IncreaseArmourInt(3);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::DecreaseFaceTypeInt()
{
	if (_CCA != NULL)
		_CCA->DecreaseArmourInt(3);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::IncreaseShouldersTypeInt()
{
	if (_CCA != NULL)
		_CCA->IncreaseArmourInt(4);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::DecreaseShouldersTypeInt()
{
	if (_CCA != NULL)
		_CCA->DecreaseArmourInt(4);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::IncreaseBodyTypeInt()
{
	if (_CCA != NULL)
		_CCA->IncreaseArmourInt(5);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::DecreaseBodyTypeInt()
{
	if (_CCA != NULL)
		_CCA->DecreaseArmourInt(5);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::IncreaseGlovesTypeInt()
{
	if (_CCA != NULL)
		_CCA->IncreaseArmourInt(6);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::DecreaseGlovesTypeInt()
{
	if (_CCA != NULL)
		_CCA->DecreaseArmourInt(6);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::IncreaseBeltTypeInt()
{
	if (_CCA != NULL)
		_CCA->IncreaseArmourInt(7);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::DecreaseBeltTypeInt()
{
	if (_CCA != NULL)
		_CCA->DecreaseArmourInt(7);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::IncreaseShoesTypeInt()
{
	if (_CCA != NULL)
		_CCA->IncreaseArmourInt(8);

	return FReply::Handled();
}

FReply SCustomizeCharacterWidget::DecreaseShoesTypeInt()
{
	if (_CCA != NULL)
		_CCA->DecreaseArmourInt(8);

	return FReply::Handled();
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef LOCTEXT_NAMESPACE
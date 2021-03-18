// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class MCOMPPROJECTCLIENT_API SCustomizeCharacterWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SCustomizeCharacterWidget){}
	SLATE_ARGUMENT(TWeakObjectPtr<class ACharacterCustomiserActor>, CCA)
	SLATE_ARGUMENT(TWeakObjectPtr<class AMainMenuHUD>, OwningHUD )
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	FReply IncreaseHeadGearInt();
	FReply DecreaseHeadGearInt();

	FReply IncreaseHairTypeInt();
	FReply DecreaseHairTypeInt();

	FReply IncreaseFaceTypeInt();
	FReply DecreaseFaceTypeInt();

	FReply IncreaseShouldersTypeInt();
	FReply DecreaseShouldersTypeInt();

	FReply IncreaseBodyTypeInt();
	FReply DecreaseBodyTypeInt();

	FReply IncreaseGlovesTypeInt();
	FReply DecreaseGlovesTypeInt();

	FReply IncreaseBeltTypeInt();
	FReply DecreaseBeltTypeInt();

	FReply IncreaseShoesTypeInt();
	FReply DecreaseShoesTypeInt();

	FReply SaveItems();

private:
	TWeakObjectPtr<class ACharacterCustomiserActor> _CCA;
	TWeakObjectPtr<class AMainMenuHUD> _OwningHUD;
};

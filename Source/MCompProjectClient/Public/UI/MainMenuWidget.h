// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class SMainMenuWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainMenuWidget) {}
	SLATE_ARGUMENT(TWeakObjectPtr<class AMainMenuHUD>, OwningHUD)
	SLATE_END_ARGS()

	/** Every widget needs a contructin function */
	void Construct(const FArguments& InArgs);

	/** the Hud that created this widget */
	TWeakObjectPtr<class AMainMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };

	FReply OnPlayClicked() const;
	FReply OnCustomizeClicked() const;
	FReply OnQuitClicked() const;
};

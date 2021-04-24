// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	/***/
	void SetHealth(float Amount) { _HealthBar->SetPercent(Amount); }

	void SetTimeLeft(const FString TimerLeftText) { _TimeLeft->SetText( FText::FromString(TimerLeftText)); }
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* _HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* _TimeLeft;


};

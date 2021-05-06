// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUW.generated.h"

/**
 * 
 */
UCLASS()
class MCOMPPROJECTCLIENT_API UMainMenuUW : public UUserWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;

	void SetKickReason(FString KickReason);
protected:
	UPROPERTY(meta = (BindWidget))
		class UButton* _PlayButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* _CustomizationButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* _QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* _RemoveKickMessageButton;

	UPROPERTY(meta = (BindWidget))
		class UImage* _KickReasonBackground;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* _KickReasonText;

	class AMainMenuHUD* _MainMenuHUD;
private:
	UFUNCTION()
		void PlayButtonPressed();

	UFUNCTION()
		void CustomizationButtonPressed();

	UFUNCTION()
		void QuitButtonPressed();

	UFUNCTION()
		void RemoveKickReason();

	void ShowKickReasonUI(bool bEnable);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Networking/MPPlayerController.h"
#include "Networking/MPPlayerState.h"
#include "Networking/MPGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "MPGameInstance.h"
#include "GameCamera.h"
#include "Networking/MPGameState.h"
#include "Networking/MPCamera.h"
#include "UI/GameHUD.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogAMPPlayerController);

AMPPlayerController::AMPPlayerController()
{
	bAutoManageActiveCameraTarget = false;
}

void AMPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() != ROLE_Authority) {}
		//Server_IncreaseVariable();

	bShowMouseCursor = false;
	bEnableClickEvents = false;
	bEnableMouseOverEvents = false;

	SetInputMode(FInputModeGameOnly());


	TArray<AActor*> _MPCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMPCamera::StaticClass(), _MPCameras);

	for (AActor* A : _MPCameras)
	{
		if (GetOwner() == A->GetOwner())
		{
			_CameraActor = Cast<AMPCamera>(A);
			//InputComponent->BindAxis("YawCameraControl", _CameraActor, &AMPCamera::YawInput);
			//InputComponent->BindAxis("PitchCameraControl", _CameraActor, &AMPCamera::PitchInput);
			break;
		}
	}

	if (AGameHUD* GHUD = Cast<AGameHUD>(GetHUD()))
	{
		UE_LOG(LogAMPPlayerController, Warning, TEXT("AGameHUD Loaded"));
		//InputComponent->BindAction("SendChatMessage", IE_Pressed, GHUD, &AGameHUD::EscapePressed);
		_GameHUD = GHUD;
		_GameHUD->EnablePreMatchStartUW(true);
	}
	else
	{
		UE_LOG(LogAMPPlayerController, Warning, TEXT("AGameHUD NOT Loaded"));
	}
}

void AMPPlayerController::Client_GameStarted_Implementation()
{
	if (_GameHUD != NULL)
	{
		_GameHUD->EnablePreMatchStartUW(false);
	}
}

void AMPPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (_CameraActor != NULL)
	{
		
	}

	InputComponent->BindAction("ChatKey", IE_Pressed, this, &AMPPlayerController::ToggleChat);
}

void AMPPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetLocalRole() != ROLE_Authority)
	{
		if (AGameHUD* HUD = Cast<AGameHUD>(GetHUD()))
		{
			if (AMPGameState* GS = Cast<AMPGameState>(UGameplayStatics::GetGameState(GetWorld())))
			{
				
				HUD->SetTimeLeft(GS->GetTimeRemaining());
			}
		}
	}

	if (_CameraActor == NULL)
	{
		//GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, TEXT("AMPPlayerConller: _CameraActor is NULL!"));
		return;
	}

	if (ACharacter* AP = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		FVector Dir = _CameraActor->GetActorLocation() - AP->GetActorLocation();
		Dir.Normalize();

		_CameraActor->SetActorLocation(AP->GetActorLocation() + (Dir * 50));
	}

	
}

void AMPPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogAMPPlayerController, Warning, TEXT("Server possessed a pawn! Changing clients camera!"));
		SetClientsCamera();
	}
}

bool AMPPlayerController::Server_SetPlayerName_Validate(const FString& Name)
{
	return true;
}

void AMPPlayerController::Server_SetPlayerName_Implementation(const FString& Name)
{

	AMPPlayerState* MPPlayerState = Cast<AMPPlayerState>(PlayerState);
	
	if (MPPlayerState != NULL)
	{
		UE_LOG(LogAMPPlayerController, Warning, TEXT("Player Changed their name!, PlayerID: %i, OldName: %s, NewName %s"),
									MPPlayerState->GetUniqueID(), *MPPlayerState->GetPlayerName(), *Name);

		MPPlayerState->SetPlayerName(Name);
	}	
}

bool AMPPlayerController::AskClientToSetName_Validate()
{
	return true;
}

void AMPPlayerController::YawInput(float Value)
{
	if (Value != 0.0f)
	{
		//GEngine->AddOnScreenDebugMessage(4, 0.5f, FColor::Blue, TEXT("YawControl"));
		if (_bHasControl)
		{
			AddYawInput(Value * _CameraSensitivity);
		}
	}
}

void AMPPlayerController::PitchInput(float Value)
{
	if (Value != 0.0f)
	{
		if (_bHasControl)
		{
			//GEngine->AddOnScreenDebugMessage(5, 0.5f, FColor::Blue, TEXT("PitchControl"));
			AddPitchInput(Value * _CameraSensitivity);
		}
	}
}

void AMPPlayerController::AskClientToSetName_Implementation()
{
	UMPGameInstance* GI = Cast<UMPGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	if (GI != NULL)
	{
		//GEngine->AddOnScreenDebugMessage(2, 5.0f, FColor::Yellow, TEXT("Client sent the server their display name"));
		Server_SetPlayerName(GI->GetPlayerName());
	}

}



//void AMPPlayerController::SendChatMessage_BP(const FString& ChatMessage)
//{
//	FChatMessage CM;
//	FString Username = PlayerState->GetPlayerName();
//	CM.SetMessage(Username, ChatMessage);
//	SendChatMessage(CM);
//}
/*
bool AMPPlayerController::SendChatMessage_Validate(const FChatMessage ChatMessage)
{
	return true;
}

void AMPPlayerController::SendChatMessage_Implementation(const FChatMessage ChatMessage)
{
	AMPGameMode* GM = Cast<AMPGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	if (GM != NULL)
	{
		UE_LOG(LogAMPPlayerController, Warning, TEXT("Client Sent a Message"));
		GM->SendChatMessageToAllClients(ChatMessage);
	}
	else
	{
		UE_LOG(LogAMPPlayerController, Error, TEXT("Cannot access GameMode"));
	}
}

bool AMPPlayerController::ClientReceiveNewChatMessage_Validate(const FChatMessage ChatMessage)
{
	return true;
}

void AMPPlayerController::ClientReceiveNewChatMessage_Implementation(const FChatMessage ChatMessage)
{
	//_LastReceivedMessage = ChatMessage;
}*/

bool AMPPlayerController::SetClientsCamera_Validate()
{
	return true;
}

void AMPPlayerController::SetClientsCamera_Implementation()
{
	UE_LOG(LogAMPPlayerController, Warning, TEXT("Changed my camera - Called From Server!"));
	SetCameraToGameCamera();
}

void AMPPlayerController::SetCameraToGameCamera()
{
	TArray<AActor*> GameCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameCamera::StaticClass(), GameCameras);
	bool bSetCamera = false;

	for (AActor* AGC : GameCameras)
	{
		SetViewTarget(AGC);
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, TEXT("Changed camera to GameCamera"));
		bSetCamera = true;
		break;
	}

	if (!bSetCamera)
	{
		//GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Yellow, TEXT("Can't find Game Camera"));

		UE_LOG(LogAMPPlayerController, Error, TEXT("Could not find game camera!"));

		if (GameCameras.Num() == 0)
			UE_LOG(LogAMPPlayerController, Error, TEXT("Game Camera TArray is NULL!"));
	}
}

//void AMPPlayerController::Client_SetHasControl(bool bHasControl)
//{ 
//	_bHasControl = bHasControl; 
//}


void AMPPlayerController::ToggleChat()
{
	if (_GameHUD != NULL)
	{
		_GameHUD->EnableChatSystem(true);
	}

	//UE_LOG(LogAMPPlayerController, Warning, TEXT("Chat Key Pressed"));
}
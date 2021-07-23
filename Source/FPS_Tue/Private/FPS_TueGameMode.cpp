// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPS_TueGameMode.h"
#include "FPS_TueHUD.h"
#include "FPS_TueCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPS_TueGameMode::AFPS_TueGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPS_TueHUD::StaticClass();
}

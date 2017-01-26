// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ldrawVRv0.h"
#include "ldrawVRv0GameMode.h"
#include "ldrawVRv0HUD.h"
#include "ldrawVRv0Character.h"

AldrawVRv0GameMode::AldrawVRv0GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AldrawVRv0HUD::StaticClass();
}

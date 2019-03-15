// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AtlantisGameMode.h"
#include "AtlantisCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAtlantisGameMode::AAtlantisGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Atlantis.PlayerCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;

	}
}

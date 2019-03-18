// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AtlantisGameMode.h"
#include "Character/AtlantisCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAtlantisGameMode::AAtlantisGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/Atlantis.PlayerCharacter"));
//	static ConstructorHelpers::FClassFinder<AHUD> HUD(TEXT("Blueprint'/Game/UI/EditorHUD.EditorHUD'"));
	
	
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;

	}
	//if (HUD.Class != NULL) {

	//	
	//	HUDClass = HUD.Class;

	////	Editor->SetWidgetClass(EDITOR.Class);
	////	Editor->SetDrawSize(FVector2D(150.0f, 50.0f));
	//}
}

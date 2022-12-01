// Copyright Epic Games, Inc. All Rights Reserved.

#include "King_Of_AnimalGameMode.h"
#include "King_Of_AnimalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKing_Of_AnimalGameMode::AKing_Of_AnimalGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

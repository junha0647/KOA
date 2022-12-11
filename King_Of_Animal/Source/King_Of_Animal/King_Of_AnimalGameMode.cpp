// Copyright Epic Games, Inc. All Rights Reserved.

#include "King_Of_AnimalGameMode.h"
#include "King_Of_AnimalCharacter.h"
#include "UObject/ConstructorHelpers.h"

AKing_Of_AnimalGameMode::AKing_Of_AnimalGameMode()
{
	numRound = 2;
	countTime = 10;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AKing_Of_AnimalGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHand, this, &AKing_Of_AnimalGameMode::CountDown, 1.0f, true);
}

void AKing_Of_AnimalGameMode::CountDown()
{
	--countTime;

	if (countTime < 1)
	{
		GetWorldTimerManager().ClearTimer(TimerHand);
		CountFinished();
	}
}

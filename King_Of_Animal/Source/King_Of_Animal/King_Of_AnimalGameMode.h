// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CPP_Character.h"
#include "King_Of_AnimalGameMode.generated.h"

UCLASS(minimalapi)
class AKing_Of_AnimalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKing_Of_AnimalGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		ACPP_Character* player1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		ACPP_Character* player2;

};



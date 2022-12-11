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

protected:
	virtual void BeginPlay() override;

public:
	AKing_Of_AnimalGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		ACPP_Character* player1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
		ACPP_Character* player2;

	void CountDown();

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode Settings")
		void CountFinished();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode Settings")
		int32 countTime;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "GameMode Settings")
		int32 numRound;	

	FTimerHandle TimerHand;
};




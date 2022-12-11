// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_Character.h"

#include "CPP_Donkey_Character.generated.h"

/**
 * 
 */
UCLASS()
class KING_OF_ANIMAL_API ACPP_Donkey_Character : public ACPP_Character
{
	GENERATED_BODY()

public:
	ACPP_Donkey_Character();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTiem) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};

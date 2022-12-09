// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Donkey_Character.h"

ACPP_Donkey_Character::ACPP_Donkey_Character()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACPP_Donkey_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_Donkey_Character::Tick(float DeltaTiem)
{
	Super::Tick(DeltaTiem);
}

void ACPP_Donkey_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


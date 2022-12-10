// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Bear_Character.h"

ACPP_Bear_Character::ACPP_Bear_Character()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACPP_Bear_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_Bear_Character::Tick(float DeltaTiem)
{
	Super::Tick(DeltaTiem);
}

void ACPP_Bear_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
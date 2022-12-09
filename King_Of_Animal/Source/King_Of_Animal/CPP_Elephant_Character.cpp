// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Elephant_Character.h"

ACPP_Elephant_Character::ACPP_Elephant_Character()
{
	PrimaryActorTick.bCanEverTick = true;


}

void ACPP_Elephant_Character::BeginPlay()
{
	Super::BeginPlay();
}

void ACPP_Elephant_Character::Tick(float DeltaTiem)
{
	Super::Tick(DeltaTiem);
}

void ACPP_Elephant_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
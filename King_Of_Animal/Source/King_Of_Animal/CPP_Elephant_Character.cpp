// Fill out your copyright notice in the Description page of Project Settings.

#include "CPP_Elephant_Character.h"

ACPP_Elephant_Character::ACPP_Elephant_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	/*
		�ۼ��� : 20181275 ������
	*/

	// Set the default values for variables.
	//characterClass = ECharacterClass::E_Elephant;

	// Create and assign the character's commands.
	characterCommands.SetNum(3);

	// Command #1 assignments.
	characterCommands[0].name = "Command #1";
	characterCommands[0].inputTypes.Add(EInputType::E_Backward);
	characterCommands[0].inputTypes.Add(EInputType::E_Forward);
	characterCommands[0].inputTypes.Add(EInputType::E_LeftPunch);
	characterCommands[0].inputs.Add("A");
	characterCommands[0].inputs.Add("D");
	characterCommands[0].inputs.Add("Y");
	characterCommands[0].hasUsedCommand = false;
	// ������

	// Command #2 assignments.
	characterCommands[1].name = "Command #2";
	characterCommands[1].inputTypes.Add(EInputType::E_Forward);
	characterCommands[1].inputTypes.Add(EInputType::E_Forward);
	characterCommands[1].inputTypes.Add(EInputType::E_RightKick);
	characterCommands[1].inputs.Add("D");
	characterCommands[1].inputs.Add("D");
	characterCommands[1].inputs.Add("J");
	characterCommands[1].hasUsedCommand = false;
	// ��������

	// Command #3 assignments.
	characterCommands[2].name = "Ult";
	characterCommands[2].inputTypes.Add(EInputType::E_Forward);
	characterCommands[2].inputTypes.Add(EInputType::E_LeftKick);
	characterCommands[2].inputTypes.Add(EInputType::E_RightKick);
	characterCommands[2].inputs.Add("D");
	characterCommands[2].inputs.Add("H");
	characterCommands[2].inputs.Add("J");
	characterCommands[2].hasUsedCommand = false;
	// �ʻ��

	command_Check = true;

	/*
		�ۼ� ����
	*/

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
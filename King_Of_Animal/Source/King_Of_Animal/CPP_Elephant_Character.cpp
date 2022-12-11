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
	characterCommands[0].inputTypes.Add(EInputType::E_Forward); // D or ��
	characterCommands[0].inputTypes.Add(EInputType::E_LeftPunch); // Y or Num8
	characterCommands[0].inputTypes.Add(EInputType::E_RightPunch); // U or Num9
	characterCommands[0].hasUsedCommand = false;
	// ������

	// Command #2 assignments.
	characterCommands[1].name = "Command #2";
	characterCommands[1].inputTypes.Add(EInputType::E_Forward); // D or ��
	characterCommands[1].inputTypes.Add(EInputType::E_Forward); // D or ��
	characterCommands[1].inputTypes.Add(EInputType::E_RightKick); // J or Num6
	characterCommands[1].hasUsedCommand = false;
	// ��������

	// Command #3 assignments.
	characterCommands[2].name = "Ult";
	characterCommands[2].inputTypes.Add(EInputType::E_Forward); // D or ��
	characterCommands[2].inputTypes.Add(EInputType::E_LeftKick); // H or Num5
	characterCommands[2].inputTypes.Add(EInputType::E_RightKick); // J or Num6
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
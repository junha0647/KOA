// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
#include "King_Of_AnimalGameMode.h"
#include "CPP_Character.h"

// Sets default values
ACPP_Character::ACPP_Character()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->GravityScale = 2.0f;

	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetRelativeRotation(FRotator(0, -90, 0));

	//PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	//PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	characterState = ECharacterState::VE_Default;
	opponent = nullptr;
	transform = FTransform();
	scale = FVector(0.0f, 0.0f, 0.0f);

	maxDistanceApart = 800.0f;
	MaxHealth = CurrentHealth;
	stunTime = 0.0f;
	pushbackDistance = 200.0f;
	launchDistance = 0.0f;
	gravityScale = GetCharacterMovement()->GravityScale;
	superMeterAmount = 0.0f;
	currentsuperMeterAmount = 100.0f;
	DamageAmount = 0.0f;
	basicsDamageAmount = 5.0f;
	SkillDamageAmount_1 = 10.0f;
	gravityScaleModifier = 0.1f;
	roundWon = 0;

	wasLightAttackUsed = false;
	wasMediumAttackUsed = false;
	wasHeavyAttackUsed = false;
	wasSuperUsed = false;
	wasLightExAttackUsed = false;
	wasMediumExAttackUsed = false;
	wasHeavyExAttackUsed = false;
	canUseExAttack = true;
	hasLostRound = false;

	PK_Check = true;
	canMove = true;
	isFlipped = false;
	IsDie = false;
	isCrouching = false;
	isuppercut = false;

	shouldGroundBounce = false;
	shouldWallBounce = false;

	// Create and assign the character's commands.
	characterCommands.SetNum(3); 

	// Command #1 assignments.
	characterCommands[0].name = "Command #1";
	//characterCommands[0].inputTypes.Add(EInputType::E_Backward); // D or ??
	characterCommands[0].inputTypes.Add(EInputType::E_Forward); // Y or Num8
	characterCommands[0].inputTypes.Add(EInputType::E_LeftPunch); // U or Num9
	//characterCommands[0].inputs.Add("A");
	characterCommands[0].inputs.Add("D");
	characterCommands[0].inputs.Add("Y");
	characterCommands[0].hasUsedCommand = false;
	// ??????

	// Command #2 assignments.
	characterCommands[1].name = "Command #2";
	characterCommands[1].inputTypes.Add(EInputType::E_Forward); // D or ??
	characterCommands[1].inputTypes.Add(EInputType::E_Forward); // D or ??
	characterCommands[1].inputTypes.Add(EInputType::E_LeftKick); // H or Num6
	characterCommands[1].inputs.Add("D");
	characterCommands[1].inputs.Add("D");
	characterCommands[1].inputs.Add("H");
	characterCommands[1].hasUsedCommand = false;
	// ????????

	// Command #3 assignments.
	characterCommands[2].name = "Ult";
	characterCommands[2].inputTypes.Add(EInputType::E_Crouch); // D or ??
	characterCommands[2].inputTypes.Add(EInputType::E_Forward); // H or Num5
	characterCommands[2].inputTypes.Add(EInputType::E_RightKick); // J or Num6
	characterCommands[2].inputs.Add("S");
	characterCommands[2].inputs.Add("D");
	characterCommands[2].inputs.Add("J");
	characterCommands[2].hasUsedCommand = false;
	// ??????

	command_Check = true;
}

// Called when the game starts or when spawned
void ACPP_Character::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (characterState != ECharacterState::VE_Jumping && characterState != ECharacterState::VE_Dead && characterState != ECharacterState::VE_WallBounce) {
		if (opponent)
		{
			if (auto characterMovement = GetCharacterMovement())
			{
				if (auto enemyMovement = opponent->GetCharacterMovement())
				{
					if (enemyMovement->GetActorLocation().Y <= characterMovement->GetActorLocation().Y)
					{
						if (isFlipped)
						{
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
							{
								transform = mesh->GetRelativeTransform();
								scale = transform.GetScale3D();
								scale.Y = -1.0f;
								transform.SetScale3D(scale);

								mesh->SetRelativeTransform(transform);
							}
							isFlipped = false;
						}
					}
					else
					{
						if (!isFlipped)
						{
							if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
							{
								transform = mesh->GetRelativeTransform();
								scale = transform.GetScale3D();
								scale.Y = 1.0f;
								transform.SetScale3D(scale);

								mesh->SetRelativeTransform(transform);
							}
							isFlipped = true;
						}
					}
				}
			}
		}
	}

	if (IsDie)
	{
		characterState = ECharacterState::VE_Dead;
	}
}

// Called to bind functionality to input
void ACPP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (auto gameMode = Cast<AKing_Of_AnimalGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (gameMode->player1 == this)
		{
			PlayerInputComponent->BindAxis("MoveRight_P1", this, &ACPP_Character::MoveRight);
			isPlayerOne = true;
		}
		else
		{
			PlayerInputComponent->BindAxis("MoveRight_P2", this, &ACPP_Character::MoveRight);
			isPlayerOne = false;
		}

		// The input mappings are ready to be used by the commands.
		NotifyPlayerLockedIn();

		PlayerInputComponent->BindAction("JumpP1", IE_Pressed, this, &ACPP_Character::Jump);
		PlayerInputComponent->BindAction("JumpP1", IE_Released, this, &ACPP_Character::StopJumping);
		PlayerInputComponent->BindAction("Crouch_P1", IE_Pressed, this, &ACPP_Character::StartCrouching);
		PlayerInputComponent->BindAction("Crouch_P1", IE_Released, this, &ACPP_Character::StopCrouching);

		// ??????
		PlayerInputComponent->BindAction("L_Punch_P1", IE_Pressed, this, &ACPP_Character::L_Punch);
		PlayerInputComponent->BindAction("R_Punch_P1", IE_Pressed, this, &ACPP_Character::R_Punch);
		PlayerInputComponent->BindAction("L_Kick_P1", IE_Pressed, this, &ACPP_Character::L_Kick);
		PlayerInputComponent->BindAction("R_Kick_P1", IE_Pressed, this, &ACPP_Character::R_Kick);

		PlayerInputComponent->BindAction("Skill_1_P1", IE_Pressed, this, &ACPP_Character::Skill_1);
		PlayerInputComponent->BindAction("Skill_2_P1", IE_Pressed, this, &ACPP_Character::Skill_2);
		PlayerInputComponent->BindAction("Skill_Ult_P1", IE_Pressed, this, &ACPP_Character::Skill_Ult);
		//PlayerInputComponent->BindAction("ExceptionalAttack_P1", IE_Pressed, this, &ACPP_Character::StartExceptionalAttack);

	}

}

void ACPP_Character::MoveRight(float axis)
{
	if (auto baseGameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
	{
		if (baseGameInstance->isDeviceForMultiplePlayers)
		{
			if (canMove && characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_Dead && characterState != ECharacterState::VE_KnockedDown && characterState != ECharacterState::VE_Recovery)
			{
				if (characterState != ECharacterState::VE_Launched && characterState != ECharacterState::VE_Jumping)
				{
					if (axis > 0.05f)
					{
						characterState = ECharacterState::VE_MovingRight;

					}
					else if (axis < -0.05f)
					{
						characterState = ECharacterState::VE_MovingLeft;
					}
					else
					{
						characterState = ECharacterState::VE_Blocking;
					}
				}

				float currentDistanceApart = abs(opponent->GetActorLocation().Y - GetActorLocation().Y);

				if ((currentDistanceApart >= maxDistanceApart))
				{
					if ((currentDistanceApart + axis < currentDistanceApart && !isFlipped) || (currentDistanceApart - axis < currentDistanceApart && isFlipped))
					{
						//AddMovementInput(GetActorForwardVector(), axis);
						AddMovementInput(FVector(0.0f, 1.0f, 0.0f), axis);
					}
				}
				else
				{
					//AddMovementInput(GetActorForwardVector(), axis);
					AddMovementInput(FVector(0.0f, 1.0f, 0.0f), axis);
				}

			}
		}
	}
}

void ACPP_Character::Jump()
{
	if (canMove && !isCrouching && characterState != ECharacterState::VE_Dead && characterState != ECharacterState::VE_Launched && characterState != ECharacterState::VE_KnockedDown && characterState != ECharacterState::VE_Recovery && characterState != ECharacterState::VE_WallBounce && characterState != ECharacterState::VE_GroundBounce)
	{
		ACharacter::Jump();
		characterState = ECharacterState::VE_Jumping;
	}
	else if (characterState == ECharacterState::VE_KnockedDown)
	{
		characterState = ECharacterState::VE_Recovery;
	}
}

void ACPP_Character::StopJumping()
{
	ACharacter::StopJumping();
}

void ACPP_Character::Landed()
{
	if (characterState == ECharacterState::VE_Launched)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("upper_End"));
		GetCharacterMovement()->GravityScale = gravityScale;
		opponent->launchDistance = 0.0f;
		characterState = ECharacterState::VE_KnockedDown;
	}
}

void ACPP_Character::L_Punch()
{
	
	if (l_punch && canMove && PK_Check && characterState != ECharacterState::VE_Dead)
	{
		PlayAnimMontage(l_punch, 1, NAME_None);
		DamageAmount = basicsDamageAmount;
		PK_Check = false;
	}
	
}

void ACPP_Character::R_Punch()
{
	if (r_punch && canMove && PK_Check && characterState != ECharacterState::VE_Dead)
	{
		PlayAnimMontage(r_punch, 1, NAME_None);
		DamageAmount = basicsDamageAmount;
		PK_Check = false;
	}
}

void ACPP_Character::L_Kick()
{
	if (l_kick && canMove && PK_Check && characterState != ECharacterState::VE_Dead)
	{
		PlayAnimMontage(l_kick, 1, NAME_None);
		DamageAmount = basicsDamageAmount;
		PK_Check = false;
	}
	
	
}

void ACPP_Character::R_Kick()
{
	
	if (r_kick && canMove && PK_Check && characterState != ECharacterState::VE_Dead)
	{
		PlayAnimMontage(r_kick, 1, NAME_None);
		DamageAmount = basicsDamageAmount;
		PK_Check = false;
	}
	
}

void ACPP_Character::Skill_1()
{
	if (skill_1 && canMove && PK_Check  && characterState != ECharacterState::VE_Dead && superMeterAmount >= 20.0f && canUseExAttack)
	{
		PlayAnimMontage(skill_1, 0.7f, NAME_None);
		wasLightAttackUsed = true;
		StartExceptionalAttack();
		PK_Check = false;
		//DamageAmount = SkillDamageAmount_1;
		launchDistance = 500.0f;
		characterCommands[0].hasUsedCommand = false;
	}
}

void ACPP_Character::Skill_2()
{
	if (skill_2 && canMove && PK_Check && characterState != ECharacterState::VE_Dead && superMeterAmount >= 50.0f && canUseExAttack)
	{
		PlayAnimMontage(skill_2, 1, NAME_None);
		wasMediumAttackUsed = true;
		StartExceptionalAttack();
		PK_Check = false;
		//DamageAmount = SkillDamageAmount_1;
		launchDistance = 700.0f;
		characterCommands[1].hasUsedCommand = false;
	}
}

void ACPP_Character::Skill_Ult()
{
	if (skill_Ult && canMove && PK_Check && characterState != ECharacterState::VE_Dead && superMeterAmount >= 100.0f && canUseExAttack)
	{
		PlayAnimMontage(skill_Ult, 1, NAME_None);
		wasHeavyAttackUsed = true;
		StartExceptionalAttack();
		PK_Check = false;
		//DamageAmount = SkillDamageAmount_1;
		characterCommands[2].hasUsedCommand = false;
	}
}

void ACPP_Character::StartExceptionalAttack()
{
	if (wasLightAttackUsed && superMeterAmount >= 20.0f && canUseExAttack)
	{
		wasLightExAttackUsed = true;
		command_Check = false;
		DamageAmount = 10.0f;
		superMeterAmount -= 20.0f;
	}
	else if (wasMediumAttackUsed && superMeterAmount >= 50.0f && canUseExAttack)
	{
		wasMediumExAttackUsed = true;
		DamageAmount = 15.0f;
		superMeterAmount -= 50.0f;
	}
	else if (wasHeavyAttackUsed && superMeterAmount >= 90.0f && canUseExAttack)
	{
		wasHeavyExAttackUsed = true;
		command_Check = false;
		DamageAmount = 7.5f;
		superMeterAmount -= 100.0f;
	}

	if (superMeterAmount < 0.00f)
	{
		superMeterAmount = 0.00f;
	}
}

void ACPP_Character::BeginStun()
{
	canMove = false;
	GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &ACPP_Character::EndStun, stunTime, false);
}

void ACPP_Character::EndStun()
{
	if (characterState != ECharacterState::VE_Launched)
	{
		characterState = ECharacterState::VE_Default;
	}
	canMove = true;
}

// ?????? ???????? ??
void ACPP_Character::PerformPushback(float pushbackAmount, float launchAmount, bool hasBlocked)
{
	if (hasBlocked)
	{
		if (isFlipped)
		{
			LaunchCharacter(FVector(0.0f, -pushbackAmount * 1.5f, 0.0f), false, false);
		}
		else
		{
			LaunchCharacter(FVector(0.0f, pushbackAmount * 1.5f, 0.0f), false, false);
		}
	}
	else
	{

		if (launchAmount > 0.0f)
		{
			characterState = ECharacterState::VE_Launched;
			isuppercut = true;
			canMove = false;
			GetCharacterMovement()->GravityScale = gravityScale * gravityScaleModifier;
			gravityScaleModifier += 0.1f;
			GetWorld()->GetTimerManager().SetTimer(uppercutTimerHandle, this, &ACPP_Character::Landed, 0.3f, false);
		}

		if (isFlipped)
		{
			LaunchCharacter(FVector(0.0f, -pushbackAmount, launchAmount), false, false);
		}
		else
		{
			LaunchCharacter(FVector(0.0f, pushbackAmount, launchAmount), false, false);
		}

	}
}

void ACPP_Character::StartCrouching()
{
	characterState = ECharacterState::VE_Crouching;
	isCrouching = true;
}

void ACPP_Character::StopCrouching()
{
	characterState = ECharacterState::VE_Default;
	isCrouching = false;
}

void ACPP_Character::StartBlocking()
{
	canMove = false;
	PK_Check = false;
	PlayAnimMontage(blocking, 1, NAME_None);
	GetWorld()->GetTimerManager().SetTimer(stunTimerHandle, this, &ACPP_Character::StopBlocking, stunTime, false);
}

void ACPP_Character::StopBlocking()
{
	characterState = ECharacterState::VE_Default;
	canMove = true;
	PK_Check = true;
	opponent->launchDistance = 0.0f;
}

void ACPP_Character::PunchReast()
{
	PlayAnimMontage(l_punchReact, 1, NAME_None);
}

// ?????? ???????? ??
void ACPP_Character::TakeDamage(float damageAmount, float hitstunTime, float blockstunTime, float pushbackAmount, float launchAmount)
{
	if (characterState != ECharacterState::VE_Dead)
	{
		if (characterState != ECharacterState::VE_Blocking)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Damege"));
			MaxHealth -= damageAmount;
			superMeterAmount += damageAmount * 1.0f;

			stunTime = hitstunTime;


			if (launchAmount > 0)
			{
				PerformPushback(pushbackAmount, launchAmount, false);
			}
			else
			{
				if (stunTime > 0.0f)
				{
					characterState = ECharacterState::VE_Stunned;
					BeginStun();
				}
				PunchReast();
				PerformPushback(pushbackAmount, launchAmount, false);
				if (!opponent->wasLightExAttackUsed)
				{
					opponent->superMeterAmount += damageAmount * 2.0f;
				}
			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Blocking"));
			float reducedDamage = damageAmount * 0.30f;
			MaxHealth -= reducedDamage;
			superMeterAmount += damageAmount * 1.5f;

			stunTime = blockstunTime;

			if (stunTime > 0.0f)
			{
				StartBlocking();
			}
			else
			{
				characterState = ECharacterState::VE_Default;
			}

			if (!opponent->wasLightExAttackUsed)
			{
				// ???? ??????????
				opponent->superMeterAmount += damageAmount * 1.15f;
			}
			/*
			if (opponent)
			{
				opponent->PerformPushback(pushbackAmount, 0.0f, false);
			}*/

			PerformPushback(pushbackAmount, launchAmount, true);
		}
	}

	if (MaxHealth <= 0.0f)
	{
		characterState = ECharacterState::VE_Dead;
		MaxHealth = 0.0f;
		canMove = false;
		IsDie = true;
	}
}

void ACPP_Character::MoveRight_P2(float value)
{
	MoveRight(value);
}

void ACPP_Character::L_Punch_P2()
{
	L_Punch();
}

void ACPP_Character::R_Punch_P2()
{
	R_Punch();
}

void ACPP_Character::L_Kick_P2()
{
	L_Kick();
}

void ACPP_Character::R_Kick_P2()
{
	R_Kick();
}

void ACPP_Character::Jump_P2()
{
	Jump();
}

void ACPP_Character::StopJump_P2()
{
	StopJumping();
}

void ACPP_Character::StartExceptionalAttack_P2()
{
	StartExceptionalAttack();
}

void ACPP_Character::Skill_1_P2()
{
	Skill_1();
}

void ACPP_Character::Skill_2_P2()
{
	Skill_2();
}

void ACPP_Character::Skill_Ult_P2()
{
	Skill_Ult();
}

//fight interface funcions
void ACPP_Character::CheckPunch_Implementation(bool is_leftHand)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("check punch"));

	FVector handLocation;
	FVector handLocation2;
	if (is_leftHand)
	{
		handLocation = GetMesh()->GetBoneLocation("LeftHand", EBoneSpaces::WorldSpace);
		handLocation2 = GetMesh()->GetBoneLocation("LeftForeArm", EBoneSpaces::WorldSpace);
	}
	else
	{
		handLocation = GetMesh()->GetBoneLocation("RightHand", EBoneSpaces::WorldSpace);
		handLocation2 = GetMesh()->GetBoneLocation("RightForeArm", EBoneSpaces::WorldSpace);
	}


	hitBone = opponent->GetClosestBone(handLocation, 80);
	hitBone2 = opponent->GetClosestBone(handLocation2, 80);

	if ((hitBone != "" || hitBone2 != "") && canMove && !opponent->isuppercut)
	{
		opponent->TakeDamage(DamageAmount, 0.3f, 0.3f, pushbackDistance, launchDistance);
	}
}

void ACPP_Character::CheckKick_Implementation(bool is_leftLeg)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("check kick"));

	FVector LegLocation;
	FVector LegLocation2;
	if (is_leftLeg)
	{
		LegLocation = GetMesh()->GetBoneLocation("LeftToe_End", EBoneSpaces::WorldSpace);
		LegLocation2 = GetMesh()->GetBoneLocation("LeftFoot", EBoneSpaces::WorldSpace);
	}
	else
	{
		LegLocation = GetMesh()->GetBoneLocation("RightToe_End", EBoneSpaces::WorldSpace);
		LegLocation2 = GetMesh()->GetBoneLocation("RightFoot", EBoneSpaces::WorldSpace);
	}


	hitBone = opponent->GetClosestBone(LegLocation, 80);
	hitBone2 = opponent->GetClosestBone(LegLocation2, 80);

	if ((hitBone != "" || hitBone2 != "") && canMove && !opponent->isuppercut)
	{
		opponent->TakeDamage(DamageAmount, 0.3f, 0.3f, pushbackDistance, launchDistance);
	}
}

FName ACPP_Character::GetClosestBone(FVector hitBonelocation, float maxDistance)
{
	TArray<FName> boneNames;
	GetMesh()->GetBoneNames(boneNames);

	FName closestBone;
	float minDist = 10000;

	for (int i = 0; i < boneNames.Num(); ++i)
	{
		FVector boneLocation = GetMesh()->GetBoneLocation(boneNames[i], EBoneSpaces::WorldSpace);
		float tempDist = FVector::Dist(hitBonelocation, boneLocation);

		if (minDist > tempDist)
		{
			minDist = tempDist;
			closestBone = boneNames[i];
		}
	}

	for (int i = 0; i < boneNames.Num(); ++i)
	{
		FVector boneLocation = GetMesh()->GetBoneLocation(boneNames[i], EBoneSpaces::WorldSpace);
		float tempDist = FVector::Dist(hitBonelocation, boneLocation);

		if (minDist > tempDist)
		{
			minDist = tempDist;
			closestBone = boneNames[i];
		}
	}

	if (minDist < maxDistance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, closestBone.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::SanitizeFloat(minDist));
		return closestBone;
	}
	else
	{
		return "";
	}

}

void ACPP_Character::CheckAttack_Implementation()
{
	PK_Check = true;
}

/*
	[ 22.12.08 ]
	?????? : 20181275 ??????
*/

void ACPP_Character::AddToInputMap(FString _input, EInputType _type)
{
	inputToInputTypeMap.Add(_input, _type);
}

void ACPP_Character::AddInputToInputBuffer(FInputInfo _inputInfo)
{
	if (!isFlipped)
	{
		if (_inputInfo.inputType == EInputType::E_Forward)
		{
			_inputInfo.inputType = EInputType::E_Backward;
		}
		else if (_inputInfo.inputType == EInputType::E_Backward)
		{
			_inputInfo.inputType = EInputType::E_Forward;
		}
	}

	inputBuffer.Add(_inputInfo);
	CheckInputBufferForCommandUsingType();
}

void ACPP_Character::CheckInputBufferForCommand()
{
	int correctSequenceCounter = 0;

	for (auto currentCommand : characterCommands)
	{
		for (int commandInput = 0; commandInput < currentCommand.inputs.Num(); ++commandInput)
		{
			for (int input = 0; input < inputBuffer.Num(); ++input)
			{
				if (input + correctSequenceCounter < inputBuffer.Num())
				{
					if (inputBuffer[input + correctSequenceCounter].inputName.Compare(currentCommand.inputs[commandInput]) == 0)
					{
						UE_LOG(LogTemp, Warning, TEXT("The player added another input to the command sequence."));
						++correctSequenceCounter;

						if (correctSequenceCounter == currentCommand.inputs.Num())
						{
							StartCommand(currentCommand.name);
						}
						break;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("The player broke the command sequence."));
						correctSequenceCounter = 0;
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("The player is not yet finished with the command sequence."));
					correctSequenceCounter = 0;
				}
			}
		}
	}
}

void ACPP_Character::CheckInputBufferForCommandUsingType()
{
	//int correctSequenceCounter = 0;

	//for (auto currentCommand : characterCommands)
	//{
	//	for (int input = 0; input < inputBuffer.Num(); ++input)
	//	{
	//		inputBuffer[input].wasUsed = false; // wasUsed?? ???? ?????
	//	}

	//	for (int commandInput = 0; commandInput < currentCommand.inputTypes.Num(); ++commandInput)
	//	{
	//		for (int input = 0; input < inputBuffer.Num(); ++input)
	//		{
	//			if (input + correctSequenceCounter < inputBuffer.Num())
	//			{
	//				if (!inputBuffer[input + correctSequenceCounter].wasUsed && inputBuffer[input + correctSequenceCounter].inputType == (currentCommand.inputTypes[commandInput]))
	//				{
	//					//UE_LOG(LogTemp, Warning, TEXT("The player added another input to the command sequence."));
	//					inputBuffer[input + correctSequenceCounter].wasUsed = true;
	//					++correctSequenceCounter;

	//					if (correctSequenceCounter == currentCommand.inputTypes.Num())
	//					{
	//						StartCommand(currentCommand.name);
	//						correctSequenceCounter = 0;
	//					}
	//					break;
	//				}
	//				else
	//				{
	//					//UE_LOG(LogTemp, Warning, TEXT("The player broke the command sequence."));
	//					correctSequenceCounter = 0;
	//				}
	//			}
	//			else
	//			{
	//				//UE_LOG(LogTemp, Warning, TEXT("The player is not yet finished with the command sequence."));
	//				correctSequenceCounter = 0;
	//			}
	//		}
	//	}
	//}

	int correctSequenceCounter = 0;

	for (auto currentCommand : characterCommands)
	{
		for (int commandInput = 0; commandInput < currentCommand.inputTypes.Num(); ++commandInput)
		{
			for (int input = 0; input < inputBuffer.Num(); ++input)
			{
				if (input + correctSequenceCounter < inputBuffer.Num())
				{
					if (inputBuffer[input + correctSequenceCounter].inputType == (currentCommand.inputTypes[commandInput]))
					{
						UE_LOG(LogTemp, Warning, TEXT("The player added another input to the command sequence."));
						++correctSequenceCounter;

						if (correctSequenceCounter == currentCommand.inputs.Num())
						{
							StartCommand(currentCommand.name);
						}
						break;
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("The player broke the command sequence."));
						correctSequenceCounter = 0;
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("The player is not yet finished with the command sequence."));
					correctSequenceCounter = 0;
				}
			}
		}
	}
}

void ACPP_Character::StartCommand(FString _commandName)
{
	for (int currentCommand = 0; currentCommand < characterCommands.Num(); ++currentCommand)
	{
		if (_commandName.Compare(characterCommands[currentCommand].name) == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("The character is using the command: %s,"), *_commandName);
			characterCommands[currentCommand].hasUsedCommand = true;
		}
	}
}

/*
	???? ????
*/

void ACPP_Character::WinRound()
{
	hasLostRound = true;
	++roundWon;
	NotifyRoundEnd();
	UpdatePointIcon();
}
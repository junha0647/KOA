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
	//characterClass = ECharacterClass::VE_Default;
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
	if (characterState != ECharacterState::VE_Jumping && characterState != ECharacterState::VE_Dead) {
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

		// 때리기
		PlayerInputComponent->BindAction("L_Punch_P1", IE_Pressed, this, &ACPP_Character::L_Punch);
		PlayerInputComponent->BindAction("R_Punch_P1", IE_Pressed, this, &ACPP_Character::R_Punch);
		PlayerInputComponent->BindAction("L_Kick_P1", IE_Pressed, this, &ACPP_Character::L_Kick);
		PlayerInputComponent->BindAction("R_Kick_P1", IE_Pressed, this, &ACPP_Character::R_Kick);

		PlayerInputComponent->BindAction("Uppercut_P1", IE_Pressed, this, &ACPP_Character::Uppercut);
		PlayerInputComponent->BindAction("Skill_4", IE_Pressed, this, &ACPP_Character::Skill_4);
		PlayerInputComponent->BindAction("ExceptionalAttack_P1", IE_Pressed, this, &ACPP_Character::StartExceptionalAttack);

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
	if (canMove && characterState != ECharacterState::VE_Dead && characterState != ECharacterState::VE_Launched && characterState != ECharacterState::VE_KnockedDown && characterState != ECharacterState::VE_Recovery)
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
	if (characterCommands[0].hasUsedCommand && canMove && command_Check && characterState != ECharacterState::VE_Dead)
	{
		wasLightAttackUsed = true;
		StartExceptionalAttack();
	}
	else if (l_punch && canMove && PK_Check && characterState != ECharacterState::VE_Dead)
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
	if (characterCommands[2].hasUsedCommand && canMove && command_Check && characterState != ECharacterState::VE_Dead)
	{
		wasHeavyAttackUsed = true;
		StartExceptionalAttack();
	}
	else if (r_kick && canMove && PK_Check && characterState != ECharacterState::VE_Dead)
	{
		PlayAnimMontage(r_kick, 1, NAME_None);
		DamageAmount = basicsDamageAmount;
		PK_Check = false;
	}
}

void ACPP_Character::Uppercut()
{
	if (canMove && command_Check && characterState != ECharacterState::VE_Dead)
	{
		PlayAnimMontage(uppercut, 0.7f, NAME_None);
		wasLightAttackUsed = true;
		PK_Check = false;
		DamageAmount = SkillDamageAmount_1;
		launchDistance = 500.0f;
		characterCommands[0].hasUsedCommand = false;
	}
}

void ACPP_Character::Skill_4()
{
	if (skill_4 && canMove && characterState != ECharacterState::VE_Dead)
	{
		PlayAnimMontage(skill_4, 1, NAME_None);
		wasHeavyAttackUsed = true;
	}
}

void ACPP_Character::StartExceptionalAttack()
{
	if (wasLightAttackUsed && superMeterAmount >= 20.0f && canUseExAttack)
	{
		wasLightExAttackUsed = true;
		Uppercut();
		command_Check = false;
		DamageAmount = 20.0f;
		superMeterAmount -= 20.0f;
	}
	else if (wasMediumAttackUsed && superMeterAmount >= 50.0f && canUseExAttack)
	{
		wasMediumExAttackUsed = true;
		DamageAmount = 25.0f;
		superMeterAmount -= 50.0f;
	}
	else if (wasHeavyAttackUsed && superMeterAmount >= 90.0f && canUseExAttack)
	{
		wasHeavyExAttackUsed = true;
		Skill_4();
		command_Check = false;
		DamageAmount = 15.0f;
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

// 적에게 적용되는 식
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

// 적에게 적용되는 식
void ACPP_Character::TakeDamage(float damageAmount, float hitstunTime, float blockstunTime, float pushbackAmount, float launchAmount)
{
	if (characterState != ECharacterState::VE_Dead)
	{
		if (characterState != ECharacterState::VE_Blocking)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Damege"));
			MaxHealth -= damageAmount;
			superMeterAmount += damageAmount * 5.85f;

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
					opponent->superMeterAmount += damageAmount * 3.0f;
				}
			}

		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Blocking"));
			float reducedDamage = damageAmount * 0.30f;
			MaxHealth -= reducedDamage;
			superMeterAmount += damageAmount * 4.0f;

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
				// 이거 잘모르겠음
				opponent->superMeterAmount += damageAmount * 0.15f;
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

void ACPP_Character::Uppercut_P2()
{
	Uppercut();
}

void ACPP_Character::StartExceptionalAttack_P2()
{
	StartExceptionalAttack();
}

//fight interface funcions
void ACPP_Character::CheckPunch_Implementation(bool is_leftHand)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("check punch"));

	FVector handLocation;
	if (is_leftHand)
	{
		handLocation = GetMesh()->GetBoneLocation("LeftHand", EBoneSpaces::WorldSpace);
	}
	else
	{
		handLocation = GetMesh()->GetBoneLocation("RightHand", EBoneSpaces::WorldSpace);
	}


	hitBone = opponent->GetClosestBone(handLocation, 80);


	if (hitBone != "" && canMove && !opponent->isuppercut)
	{
		opponent->TakeDamage(DamageAmount, 0.3f, 0.3f, pushbackDistance, launchDistance);
	}
}

void ACPP_Character::CheckKick_Implementation(bool is_leftLeg)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("check kick"));

	FVector LegLocation;
	if (is_leftLeg)
	{
		LegLocation = GetMesh()->GetBoneLocation("LeftToe_End", EBoneSpaces::WorldSpace);
	}
	else
	{
		LegLocation = GetMesh()->GetBoneLocation("RightToe_End", EBoneSpaces::WorldSpace);
	}


	hitBone = opponent->GetClosestBone(LegLocation, 80);


	if (hitBone != "" && canMove && !opponent->isuppercut)
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
	작성자 : 20181275 조준하
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
	//int correctSequenceCounter = 0;

	//for (auto currentCommand : characterCommands)
	//{
	//	for (int commandInput = 0; commandInput < currentCommand.inputs.Num(); ++commandInput)
	//	{
	//		for (int input = 0; input < inputBuffer.Num(); ++input)
	//		{
	//			if (input + correctSequenceCounter < inputBuffer.Num())
	//			{
	//				if (inputBuffer[input + correctSequenceCounter].inputName.Compare(currentCommand.inputs[commandInput]) == 0)
	//				{
	//					UE_LOG(LogTemp, Warning, TEXT("The player added another input to the command sequence."));
	//					++correctSequenceCounter;

	//					if (correctSequenceCounter == currentCommand.inputs.Num())
	//					{
	//						StartCommand(currentCommand.name);
	//					}
	//					break;
	//				}
	//				else
	//				{
	//					UE_LOG(LogTemp, Warning, TEXT("The player broke the command sequence."));
	//					correctSequenceCounter = 0;
	//				}
	//			}
	//			else
	//			{
	//				UE_LOG(LogTemp, Warning, TEXT("The player is not yet finished with the command sequence."));
	//				correctSequenceCounter = 0;
	//			}
	//		}
	//	}
	//}
}

void ACPP_Character::CheckInputBufferForCommandUsingType()
{
	int correctSequenceCounter = 0;

	for (auto currentCommand : characterCommands)
	{
		for (int input = 0; input < inputBuffer.Num(); ++input)
		{
			inputBuffer[input].wasUsed = false; // wasUsed는 무슨 변수?
		}

		for (int commandInput = 0; commandInput < currentCommand.inputTypes.Num(); ++commandInput)
		{
			for (int input = 0; input < inputBuffer.Num(); ++input)
			{
				if (input + correctSequenceCounter < inputBuffer.Num())
				{
					if (!inputBuffer[input + correctSequenceCounter].wasUsed && inputBuffer[input + correctSequenceCounter].inputType == (currentCommand.inputTypes[commandInput]))
					{
						//UE_LOG(LogTemp, Warning, TEXT("The player added another input to the command sequence."));
						inputBuffer[input + correctSequenceCounter].wasUsed = true;
						++correctSequenceCounter;

						if (correctSequenceCounter == currentCommand.inputTypes.Num())
						{
							StartCommand(currentCommand.name);
							correctSequenceCounter = 0;
						}
						break;
					}
					else
					{
						//UE_LOG(LogTemp, Warning, TEXT("The player broke the command sequence."));
						correctSequenceCounter = 0;
					}
				}
				else
				{
					//UE_LOG(LogTemp, Warning, TEXT("The player is not yet finished with the command sequence."));
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
	작성 종료
*/

void ACPP_Character::WinRound()
{
	hasLostRound = true;
	++roundWon;
	NotifyRoundEnd();
	UpdatePointIcon();
}
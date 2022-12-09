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

	wasLightAttackUsed = false;
	wasMediumAttackUsed = false;
	wasHeavyAttackUsed = false;
	wasSuperUsed = false;
	wasLightExAttackUsed = false;
	wasMediumExAttackUsed = false;
	wasHeavyExAttackUsed = false;
	canUseExAttack = true;

	PK_Check = true;
	canMove = true;
	isFlipped = false;
	IsDie = false;
	isCrouching = false;
	isuppercut = false;

	/*
		[ 22.12.08 ]
		작성자 : 20181275 조준하
	*/

	removeInputFromBufferTime = 1.0f;
	
	tempCommand.name = "Temp Command";
	tempCommand.inputs.Add("A");
	tempCommand.inputs.Add("B");
	tempCommand.inputs.Add("C");
	hasUsedTempCommand = false;

	// Create and assign the character's commands.
	characterCommands.SetNum(2);

	// Command #1 assignments.
	characterCommands[0].name = "Command #1";
	characterCommands[0].inputs.Add("D");
	characterCommands[0].inputs.Add("Y");
	characterCommands[0].inputs.Add("U");
	characterCommands[0].hasUsedCommand = false;

	// Command #2 assignments.
	characterCommands[1].name = "Command #2";
	characterCommands[1].inputs.Add("A");
	characterCommands[1].inputs.Add("B");
	characterCommands[1].inputs.Add("D");
	characterCommands[1].hasUsedCommand = false;

	command_Check = true;

	/*
		작성 종료
	*/
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
	if (auto gameMode = Cast<AKing_Of_AnimalGameMode>(GetWorld()->GetAuthGameMode())) {
		if (gameMode->player1 == this)
		{
			PlayerInputComponent->BindAction("JumpP1", IE_Pressed, this, &ACPP_Character::Jump);
			PlayerInputComponent->BindAction("JumpP1", IE_Released, this, &ACPP_Character::StopJumping);
			PlayerInputComponent->BindAction("Crouch_P1", IE_Pressed, this, &ACPP_Character::StartCrouching);
			PlayerInputComponent->BindAction("Crouch_P1", IE_Released, this, &ACPP_Character::StopCrouching);

			PlayerInputComponent->BindAxis("MoveRight_P1", this, &ACPP_Character::MoveRight);
			// 때리기
			PlayerInputComponent->BindAction("L_Punch_P1", IE_Pressed, this, &ACPP_Character::L_Punch);
			PlayerInputComponent->BindAction("R_Punch_P1", IE_Pressed, this, &ACPP_Character::R_Punch);
			PlayerInputComponent->BindAction("L_Kick_P1", IE_Pressed, this, &ACPP_Character::L_Kick);
			PlayerInputComponent->BindAction("R_Kick_P1", IE_Pressed, this, &ACPP_Character::R_Kick);
			PlayerInputComponent->BindAction("Uppercut_P1", IE_Pressed, this, &ACPP_Character::Uppercut);
			PlayerInputComponent->BindAction("Skill_4", IE_Pressed, this, &ACPP_Character::Skill_4);

			PlayerInputComponent->BindAction("ExceptionalAttack_P1", IE_Pressed, this, &ACPP_Character::StartExceptionalAttack);
		
			/*
				[ 22.12.08 ]
				작성자 : 20181275 조준하
			*/

			// This will work for Player 1 assuming their inputs are filltered to only controls that they can use.
			// PlayerInputComponent->BindAction("AddToInputBuffer", IE_Pressed, this, &ACPP_Character::AddInputToInputBuffer);
		
			/*
				작성 종료
			*/
		}
		else
		{
			PlayerInputComponent->BindAction("JumpP2", IE_Pressed, this, &ACPP_Character::Jump);
			PlayerInputComponent->BindAction("JumpP2", IE_Released, this, &ACPP_Character::StopJumping);
			PlayerInputComponent->BindAction("Crouch_P2", IE_Pressed, this, &ACPP_Character::StartCrouching);
			PlayerInputComponent->BindAction("Crouch_P2", IE_Released, this, &ACPP_Character::StopCrouching);

			PlayerInputComponent->BindAxis("MoveRight_P2", this, &ACPP_Character::MoveRight);
			// 때리기
			PlayerInputComponent->BindAction("L_Punch_P2", IE_Pressed, this, &ACPP_Character::L_Punch);
			PlayerInputComponent->BindAction("R_Punch_P2", IE_Pressed, this, &ACPP_Character::R_Punch);
			PlayerInputComponent->BindAction("L_Kick_P2", IE_Pressed, this, &ACPP_Character::L_Kick);
			PlayerInputComponent->BindAction("R_Kick_P2", IE_Pressed, this, &ACPP_Character::R_Kick);
			PlayerInputComponent->BindAction("Uppercut_P2", IE_Pressed, this, &ACPP_Character::Uppercut);

			PlayerInputComponent->BindAction("ExceptionalAttack_P2", IE_Pressed, this, &ACPP_Character::StartExceptionalAttack);
		
			/*
				[ 22.12.08 ]
				작성자 : 20181275 조준하
			*/

			// This will work for Player 2 assuming their inputs are filltered to only controls that they can use.
			// PlayerInputComponent->BindAction("AddToInputBuffer", IE_Pressed, this, &ACPP_Character::AddInputToInputBuffer);

			/*
				작성 종료
			*/
		}
	}

}

void ACPP_Character::MoveRight(float axis)
{
	if (canMove && characterState != ECharacterState::VE_Crouching && characterState != ECharacterState::VE_Dead)
	{
		if (characterState != ECharacterState::VE_Launched && characterState != ECharacterState::VE_Jumping) {
			if (axis < 0.05f && axis > -0.05f)
			{
				characterState = ECharacterState::VE_Blocking;

			}
			else
			{
				characterState = ECharacterState::VE_Default;
			}
		}


		float currentDistanceApart = abs(opponent->GetActorLocation().Y - GetActorLocation().Y);

		if ((currentDistanceApart >= maxDistanceApart))
		{
			if ((currentDistanceApart + axis < currentDistanceApart) || (currentDistanceApart - axis < currentDistanceApart))
			{
				AddMovementInput(GetActorForwardVector(), axis);
			}
		}
		else
		{
			AddMovementInput(GetActorForwardVector(), axis);
		}

	}
}

void ACPP_Character::Jump()
{
	if (canMove && characterState != ECharacterState::VE_Dead && characterState != ECharacterState::VE_Launched)
	{
		ACharacter::Jump();
		characterState = ECharacterState::VE_Jumping;
	}
}

void ACPP_Character::StopJumping()
{
	ACharacter::StopJumping();
}

void ACPP_Character::Landed()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("upper_End"));
	GetCharacterMovement()->GravityScale = gravityScale;
	opponent->launchDistance = 0.0f;
	characterState = ECharacterState::VE_Default;
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
	if (characterCommands[0].hasUsedCommand && canMove && command_Check && characterState != ECharacterState::VE_Dead)
	{
		Uppercut();
	}
	else if (r_punch && canMove && PK_Check && characterState != ECharacterState::VE_Dead)
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

void ACPP_Character::Uppercut()
{
	if (canMove && characterState != ECharacterState::VE_Dead)
	{
		PlayAnimMontage(uppercut, 0.7f, NAME_None);
		wasLightAttackUsed = true;
		PK_Check = false;
		command_Check = false;
		DamageAmount = SkillDamageAmount_1;
		launchDistance = 500.0f;
		characterCommands[0].hasUsedCommand = false;
	}
}

void ACPP_Character::Skill_4()
{
	if (superMeterAmount >= 100.0f)
	{
		if (skill_4 && canMove && PK_Check && characterState != ECharacterState::VE_Dead)
		{
			PlayAnimMontage(skill_4, 1, NAME_None);
		}
	}
}

void ACPP_Character::StartExceptionalAttack()
{
	if (wasLightAttackUsed && superMeterAmount >= 20.0f && canUseExAttack)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("LightAttack"));
		wasLightExAttackUsed = true;
		DamageAmount = 20.0f;
		superMeterAmount -= 20.0f;
	}
	else if (wasMediumAttackUsed && superMeterAmount >= 35.0f && canUseExAttack)
	{
		wasMediumExAttackUsed = true;
		superMeterAmount -= 35.0f;
	}
	else if (wasHeavyAttackUsed && superMeterAmount >= 50.0f && canUseExAttack)
	{
		wasHeavyExAttackUsed = true;
		superMeterAmount -= 50.0f;
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
			GetCharacterMovement()->GravityScale *= 0.5f;
			GetWorld()->GetTimerManager().SetTimer(uppercutTimerHandle, this, &ACPP_Character::Landed, 0.1f, false);
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
			superMeterAmount += damageAmount * 8.5f;   

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
			float reducedDamage = damageAmount * 3.0f;
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

void ACPP_Character::AddInputToInputBuffer(FInputInfo _inputInfo)
{
	inputBuffer.Add(_inputInfo);
	CheckInputBufferForCommand();
	// GetWorld()->GetTimerManager().SetTimer(inputBufferTimerHandle, this, &ACPP_Character::RemoveInputFromInputBuffer, removeInputFromBufferTime, false);
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

void ACPP_Character::RemoveInputFromInputBuffer()
{

}

/*
	작성 종료
*/
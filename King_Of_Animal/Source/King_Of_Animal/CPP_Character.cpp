// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/CapsuleComponent.h"
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
	

	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->SetRelativeRotation(FRotator(0, -90, 0));


	//PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	//PlayerCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	PK_Check = true;
	IsDown = false;
	MaxHealth = CurrentHealth;

	transform = FTransform();
	scale = FVector(0.0f, 0.0f, 0.0f);
	isFlipped = false;
	//isPlayer_1_2 = false;
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

	if (opponent)
	{
		if (auto characterMovement = GetCharacterMovement())
		{
			if (auto enemyMovement = opponent->GetCharacterMovement())
			{
				if (enemyMovement->GetActorLocation().Y >= characterMovement->GetActorLocation().Y)
				{
					if (isFlipped)
					{
						if (auto mesh = GetCapsuleComponent()->GetChildComponent(1))
						{
							transform = mesh->GetRelativeTransform();
							scale = transform.GetScale3D();
							scale.Y = -0.3f;
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
							scale.Y = 0.3f;
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

// Called to bind functionality to input
void ACPP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!isPlayer_1_2) {
		PlayerInputComponent->BindAction("JumpP1", IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction("JumpP1", IE_Released, this, &ACharacter::StopJumping);

		PlayerInputComponent->BindAxis("MoveRight_P1", this, &ACPP_Character::MoveRight);
		// 때리기
		PlayerInputComponent->BindAction("L_Punch_P1", IE_Pressed, this, &ACPP_Character::L_Punch);
		PlayerInputComponent->BindAction("R_Punch_P1", IE_Pressed, this, &ACPP_Character::R_Punch);
		PlayerInputComponent->BindAction("L_Kick_P1", IE_Pressed, this, &ACPP_Character::L_Kick);
		PlayerInputComponent->BindAction("R_Kick_P1", IE_Pressed, this, &ACPP_Character::R_Kick);
	}
	else
	{
		PlayerInputComponent->BindAction("JumpP2", IE_Pressed, this, &ACharacter::Jump);
		PlayerInputComponent->BindAction("JumpP2", IE_Released, this, &ACharacter::StopJumping);

		PlayerInputComponent->BindAxis("MoveRight_P2", this, &ACPP_Character::MoveRight);
		// 때리기
		PlayerInputComponent->BindAction("L_Punch_P2", IE_Pressed, this, &ACPP_Character::L_Punch);
		PlayerInputComponent->BindAction("R_Punch_P2", IE_Pressed, this, &ACPP_Character::R_Punch);
		PlayerInputComponent->BindAction("L_Kick_P2", IE_Pressed, this, &ACPP_Character::L_Kick);
		PlayerInputComponent->BindAction("R_Kick_P2", IE_Pressed, this, &ACPP_Character::R_Kick);
	}
}

void ACPP_Character::MoveRight(float axis)
{
	if (!IsDown) 
	{
		AddMovementInput(GetActorForwardVector(), axis);
	}
}

void ACPP_Character::L_Punch()
{
	if (l_punch && PK_Check && !IsDown)
	{
		PlayAnimMontage(l_punch, 1, NAME_None);
		PK_Check = false;
	}
}

void ACPP_Character::R_Punch()
{
	if (r_punch && PK_Check && !IsDown)
	{
		PlayAnimMontage(r_punch, 1, NAME_None);
		PK_Check = false;
	}
}

void ACPP_Character::L_Kick()
{
	if (l_kick && PK_Check && !IsDown)
	{
		PlayAnimMontage(l_kick, 1, NAME_None);
		PK_Check = false;
	}
}

void ACPP_Character::R_Kick()
{
	if (r_kick && PK_Check && !IsDown)
	{
		PlayAnimMontage(r_kick, 1, NAME_None);
		PK_Check = false;
	}
}

void ACPP_Character::PunchReast()
{
	PlayAnimMontage(l_punchReact, 1, NAME_None);
}

void ACPP_Character::TakeDamage(float damageAmount)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, TEXT("Damege"));
	MaxHealth -= damageAmount;
	
	if (MaxHealth <= 0.0f)
	{
		MaxHealth = 0.0f;
		IsDown = true;
		GetCharacterMovement()->JumpZVelocity = 0.f;
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

	FName hitBone = opponent->GetClosestBone(handLocation, 80);

	if (hitBone != "" && !IsDown)
	{
		opponent->PunchReast();
		opponent->TakeDamage(10.0f);
	}
}

void ACPP_Character::CheckKick_Implementation(bool is_leftLeg)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("check kick"));

	FVector LegLocation;
	if (is_leftLeg)
	{
		LegLocation = GetMesh()->GetBoneLocation("LeftLeg", EBoneSpaces::WorldSpace);
	}
	else
	{
		LegLocation = GetMesh()->GetBoneLocation("RightLeg", EBoneSpaces::WorldSpace);
	}

	FName hitBone = opponent->GetClosestBone(LegLocation, 80);

	if (hitBone != "" && !IsDown)
	{
		opponent->PunchReast();
		TakeDamage(10.0f);
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

// end of fight interface



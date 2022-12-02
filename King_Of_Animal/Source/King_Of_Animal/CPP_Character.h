// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "FightInterface.h"

#include "CPP_Character.generated.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	VE_Default   UMETA(DisplayName = "NOT_MOVING"),
	VE_Jumping   UMETA(DisplayName = "JUMPING")
};

UCLASS()
class KING_OF_ANIMAL_API ACPP_Character : public ACharacter, public IFightInterface
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	ACPP_Character();

	
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	//USpringArmComponent* CameraBoom;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	//UCameraComponent* PlayerCamera;

	virtual void Jump() override;
	virtual void StopJumping() override;
	void MoveRight(float axis);
	void L_Punch();
	void R_Punch();
	void L_Kick();
	void R_Kick();
	void PunchReast();
	void TakeDamage(float damageAmount);

	// Player 2
	UFUNCTION(BlueprintCallable)
	void MoveRight_P2(float value);
	UFUNCTION(BlueprintCallable)
	void L_Punch_P2();
	UFUNCTION(BlueprintCallable)
	void R_Punch_P2();
	UFUNCTION(BlueprintCallable)
	void L_Kick_P2();
	UFUNCTION(BlueprintCallable)
	void R_Kick_P2();
	UFUNCTION(BlueprintCallable)
	void Jump_P2();
	UFUNCTION(BlueprintCallable)
	void StopJump_P2();

	FName GetClosestBone(FVector hitBonelocation, float maxDistance);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* l_punch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* r_punch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* l_kick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* r_kick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* l_punchReact;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ACPP_Character* opponent;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FTransform transform;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FVector scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		ECharacterState characterState;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check")
	bool PK_Check;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check")
	bool IsDown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	bool isFlipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player_Choice")
	bool isPlayer_1_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float maxDistanceApart;

public:
	FName hitBone;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	// fight interface functions
	// punch hit check
	void CheckPunch_Implementation(bool is_leftHand) override;

	void CheckKick_Implementation(bool is_leftLeg) override;

	void CheckAttack_Implementation() override;

	// end of fight interface

};

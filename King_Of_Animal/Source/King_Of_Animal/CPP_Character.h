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
	VE_Jumping   UMETA(DisplayName = "JUMPING"),
	VE_Stunned   UMETA(DisplayName = "STUNNED"),
	VE_Blocking  UMETA(DisplayName = "BLOCKING"),
	VE_Crouching UMETA(DisplayName = "CROUCHING"),
	VE_Dead      UMETA(DisplayName = "DEAD"),
	VE_Launched  UMETA(DisplayName = "LAUNCHED")
};

UCLASS()
class KING_OF_ANIMAL_API ACPP_Character : public ACharacter, public IFightInterface
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	ACPP_Character();


	// Player Jump
	virtual void Jump() override;
	virtual void StopJumping() override;
	// Player Move
	void MoveRight(float axis);
	// Player attack
	void L_Punch();
	void R_Punch();
	void L_Kick();
	void R_Kick();
	void Uppercut();
	void StartExceptionalAttack();

	// player Hit
	void PunchReast();
	// Damage the play
	void TakeDamage(float damageAmount, float hitstunTime, float blockstunTime, float pushbackAmount, float launchAmount);
	//Enter the stun-state
	void BeginStun();
	//Exit the stun-state
	void EndStun();
	// Determin how for the characters should be pushed back
	void PerformPushback(float pushbackAmount, float launchAmount, bool hasBlocked);

	// Character crouching
	UFUNCTION(BlueprintCallable)
	void StartCrouching();
	UFUNCTION(BlueprintCallable)
	void StopCrouching();

	// Character blocking
	UFUNCTION(BlueprintCallable)
	void StartBlocking();
	UFUNCTION(BlueprintCallable)
	void StopBlocking();

	UFUNCTION(BlueprintCallable)
	void Landed();

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
	UFUNCTION(BlueprintCallable)
	void Uppercut_P2();
	UFUNCTION(BlueprintCallable)
	void StartExceptionalAttack_P2();



	FName GetClosestBone(FVector hitBonelocation, float maxDistance);

public:
	// player AnimMontage
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* blocking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* uppercut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player References")
	ACPP_Character* opponent;
	
	// the character's transform.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FTransform transform;

	// The character's scale
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	FVector scale;

	// The current state of the character (jumping, stun, and more)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	ECharacterState characterState;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check")
	bool PK_Check;

	// player Move State Check
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check")
	bool canMove;

	// player Die Check
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Check")
	bool IsDie;

	// Player Flip check and change
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Model")
	bool isFlipped;

	// Player crouch check.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool isCrouching;

	// Enemy uppercut check.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool isuppercut;

	// Has the player used the light attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasLightAttackUsed;

	// Has the player used the medium attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasMediumAttackUsed;

	// Has the player used the heavy attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasHeavyAttackUsed;

	// Has the player used the super attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasSuperUsed;

	// Has the player used the medium exceptional attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasLightExAttackUsed;

	// Has the player used the heavy exceptional attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasMediumExAttackUsed;

	// Has the player used the super exceptional attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool wasHeavyExAttackUsed;

	// The amount of health the character currently has.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth = 100.0f;

	// The maximum amount of distance that the characters can be apart.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float maxDistanceApart;

	// The amount of thim the character will be stunned.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float stunTime;

	// The amount of thim the character will be stunned.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float pushbackDistance;

	// The amount of distance to launch the player.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float launchDistance;

	// The scaled value of gravity.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float gravityScale;

	// The amount of super meter thr player has.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Super Meter")
		float superMeterAmount;

public:
	FName hitBone;
	
	// The timer handle for all stuns
	FTimerHandle stunTimerHandle;

	// The timer handle for all 
	FTimerHandle uppercutTimerHandle;

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

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "FightInterface.h"
#include "BaseGameInstance.h"

#include "CPP_Character.generated.h"



UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	VE_Default       UMETA(DisplayName = "NOT_MOVING"),
	VE_MovingRight   UMETA(DisplayName = "MOVINGRIGHT"),
	VE_MovingLeft    UMETA(DisplayName = "MOVINGLEFT"),
	VE_Jumping       UMETA(DisplayName = "JUMPING"),
	VE_Stunned       UMETA(DisplayName = "STUNNED"),
	VE_Blocking      UMETA(DisplayName = "BLOCKING"),
	VE_Crouching     UMETA(DisplayName = "CROUCHING"),
	VE_Dead          UMETA(DisplayName = "DEAD"),
	VE_Launched      UMETA(DisplayName = "LAUNCHED"),
	VE_KnockedDown   UMETA(DisplayName = "KNOCKED DOWN"),
	VE_Recovery      UMETA(DisplayName = "RECOVERY"),
	VE_WallBounce    UMETA(DisplayName = "WALL BOUNCED"),
	VE_GroundBounce  UMETA(DisplayName = "GROUND BOUNCED")
};

/*
	작성자 : 20181275 조준하
*/

UENUM(BlueprintType)
enum class EInputType : uint8
{
	E_None			UMETA(DisplayName = "NONE"),
	E_Forward		UMETA(DisplayName = "FORWARD"),
	E_Backward		UMETA(DisplayName = "BACKWARD"),
	E_Jump			UMETA(DisplayName = "JUMP"),
	E_Crouch		UMETA(DisplayName = "CROUCH"),
	E_LeftPunch		UMETA(DisplayName = "LEFTPUNCH"),
	E_RightPunch	UMETA(DisplayName = "RIGHTPUNCH"),
	E_LeftKick		UMETA(DisplayName = "LEFTKICK"),
	E_RightKick		UMETA(DisplayName = "RIGHTKICK")
};

USTRUCT(BlueprintType)
struct FCommand
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FString name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<EInputType> inputTypes;

	// new
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FString> inputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool hasUsedCommand;
};

USTRUCT(BlueprintType)
struct FInputInfo
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	EInputType inputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FString inputName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float timeStamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	bool wasUsed;
};

/*
	작성 종료
*/

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
	void StartExceptionalAttack();
	void Skill_1();
	void Skill_2();
	void Skill_Ult();

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
	void StartExceptionalAttack_P2();
	UFUNCTION(BlueprintCallable)
	void Skill_1_P2();
	UFUNCTION(BlueprintCallable)
	void Skill_2_P2();
	UFUNCTION(BlueprintCallable)
	void Skill_Ult_P2();



	FName GetClosestBone(FVector hitBonelocation, float maxDistance);

	/*
		작성자 : 20181275 조준하
	*/

	UFUNCTION(BlueprintCallable)
	void AddToInputMap(FString _input, EInputType _type);

	// Adds inputs to the input buffer.
	UFUNCTION(BlueprintCallable)
	void AddInputToInputBuffer(FInputInfo _inputInfo);

	// Check if the input buffer contains any sequences from the character's list of commands.
	UFUNCTION(BlueprintCallable)
	void CheckInputBufferForCommand();

	// Check if the input buffer contains any sequences from the character's list of commands using input types.
	UFUNCTION(BlueprintCallable)
	void CheckInputBufferForCommandUsingType();

	// Make the character begin using a command based off of the command's name.
	UFUNCTION(BlueprintCallable)
	void StartCommand(FString _commandName);

	UFUNCTION(BlueprintImplementableEvent)
	void NotifyPlayerLockedIn();

	// The map of inputs-to-InputTypes for access when determining if the player controlling this character has used a recognized input.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TMap<FString, EInputType> inputToInputTypeMap;

	// The array of inputs the player controlling this character has performed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FInputInfo> inputBuffer;

	// Commands to be used when a correct series of inputs has been pressed.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<FCommand> characterCommands;

	// The timer handle to remove inputs from the input buffer.
	FTimerHandle inputBufferTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool isDeviceForMultiplePlayers;
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Commands")
	bool command_Check;

	/*
		작성 종료
	*/

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
	UAnimMontage* skill_1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* skill_2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* skill_Ult;

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

	// Has the player used the super exceptional attack?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	bool canUseExAttack;

	// Has the aharacter been thrown?
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	//bool wasThrown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Stack")
	bool isPlayerOne;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool shouldGroundBounce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool shouldWallBounce;

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

	// Player Attack Damage
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attacks")
	float DamageAmount;

	// Player basics Attack Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	float basicsDamageAmount;

	// Player skill_1 Attack Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacks")
	float SkillDamageAmount_1;

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

	// The amount of super meter thr player has.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Super Meter")
	float currentsuperMeterAmount;

	// The amount of super meter thr player has.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float gravityScaleModifier;

public:
	FName hitBone;
	FName hitBone2;
	
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

	UFUNCTION(BlueprintCallable)
		void WinRound();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void NotifyRoundStart();

	UFUNCTION(BlueprintImplementableEvent)
		void NotifyRoundEnd();

	UFUNCTION(BlueprintImplementableEvent)
		void UpdatePointIcon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int roundWon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool hasLostRound;
};

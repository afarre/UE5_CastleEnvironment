// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "ParentEnemy.h"
#include "SkeletonSwordsman.h"
#include "GameFramework/Character.h"
#include "Pirate.generated.h"

// Forward declarations
class USoundCue;
class AWarMace;
class AParentWeapon;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class APrimaryHUD;
class UAnimMontage;
class UAnimInstance;
struct FInputActionValue;
struct FTimerHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDefaultAnimationDelegate);
DECLARE_DELEGATE(FTestDelegate);

UCLASS()
class CASTLEENVIRONMENT_API APirate : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APirate();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpdateWidgetHP(APrimaryHUD* PrimaryHUD);
	
	void AttackConnected(AActor* AttackConnectedActor);

	void ActOnConnectedAttacks();

	bool IsInteracting;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	mutable float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	mutable float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	mutable float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	mutable float BaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	mutable float WalkModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	mutable float SprintModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	mutable float MovementModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stamina)
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stamina)
	float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stamina)
	float StaminaRegenRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stamina)
	float StaminaDepleteRate;
	
	const AWarMace* WieldedWeapon;
	
	FTestDelegate TestDelegate;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* EffortGrunt;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio, meta = (AllowPrivateAccess = "true"))
	USoundCue* AttackSlash;

	UAudioComponent* EffortGruntAudioComponent;
	UAudioComponent* AttackSlashAudioComponent;
	
protected:
	virtual void BeginPlay() override;

	// Camera elements
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Components)
	UCameraComponent* FollowCamera;
	
	// Create variable to select an Input Mapping Context (IMC_PirateContext) in the editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	// ReSharper disable once UnrealHeaderToolError
	UInputMappingContext* PirateMappingContext;
	
	// Create variable to select an InputAction for an action in the editor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* WheelAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* TestAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* AttackAction;
	
	// Define functions with the value inputted when playing
	void Move(const FInputActionValue& Value);
	void MoveEnd();
	
	void Look(const FInputActionValue& Value);

	void Jump();

	void SprintOngoing();
	void SprintEnd();

	void CameraZoom(const FInputActionValue& Value);

	void StartInteract(const FInputActionValue& Value);
	void InteractEnd(const FInputActionValue& Value);
	
	void Attack();
	
	// Variables
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovementComponent;

	// Camera zoom variables
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float MinCameraZoom = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float MaxCameraZoom = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float DefaultArmLength = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float ZoomStep = 20.0f;

	FVector MaxCameraHeight = FVector(0, 0, MaxCameraZoom/2);
	FVector MinCameraHeight = FVector(0, 0, MinCameraZoom);

	// My Game State Base pointer (controller)
	AMyGameStateBase* MyGameStateBase;

	// Slash attach animation montage for this character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Animation)
	UAnimMontage* SlashAttackAnimMontage;

private:
	// Move and sprint flow control boolean variables
	bool IsSprinting;
	bool CanSprint;
	bool IsMoving;
	bool AttackConnectedFlag;
	TArray<AActor*> ConnectedEnemies;

	UFUNCTION()
	void EnableSprinting();
	
	void SprintCooldown();

	FTimerHandle TimerHandle;
	float TimerDelay;

	UPROPERTY(EditDefaultsOnly, Category= Attack)
	UAnimationAsset* AttackAnimation;

	UAnimInstance* AnimInstance;
	
};

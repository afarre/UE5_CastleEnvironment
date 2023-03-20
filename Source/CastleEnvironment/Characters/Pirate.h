// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "ParentEnemy.h"
#include "GameFramework/Character.h"
#include "Pirate.generated.h"

// Forward declarations
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class APrimaryHUD;
struct FInputActionValue;

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
	
	// Define functions with the value inputted when playing
	void Move(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);

	void Jump(const FInputActionValue& Value);

	void SprintOngoing(const FInputActionValue& Value);
	void SprintCanceled(const FInputActionValue& Value);
	void SprintCompleted(const FInputActionValue& Value);

	void CameraZoom(const FInputActionValue& Value);

	void StartInteract(const FInputActionValue& Value);
	void CanceledInteract(const FInputActionValue& Value);
	void CompletedInteract(const FInputActionValue& Value);

	void TestInteraction(const FInputActionValue& Value);

	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
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

	AMyGameStateBase* MyGameStateBase;

private:
	float IsSprinting;
};
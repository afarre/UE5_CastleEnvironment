// Fill out your copyright notice in the Description page of Project Settings.


#include "Pirate.h"

#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "VectorTypes.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CastleEnvironment/MyGameStateBase.h"
#include "CastleEnvironment/UI/PrimaryHUD.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

// Sets default values
APirate::APirate() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Auto Posses for this Pawn
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	//SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MyMesh"));
	//SkeletalMeshComponent->SetupAttachment(RootComponent);

	//Initialize the Camera Boom    
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	//Setup Camera Boom attachment to the SkeletalMeshComponent of the class
	CameraBoom->SetupAttachment(GetCapsuleComponent(), "headSocket");

	//Set the boolean to use the PawnControlRotation to true.
	CameraBoom->bUsePawnControlRotation = true;

	// Set camera arm length
	CameraBoom->TargetArmLength = DefaultArmLength;

	//Initialize the FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	//Set FollowCamera attachment to the Camera Boom
	FollowCamera->SetupAttachment(CameraBoom);

	CurrentHealth = MaxHealth = 1.0f;
	MovementModifier = WalkModifier = .25f;
	SprintModifier = 1.0f;
	BaseSpeed = 1200.0f;
	BaseDamage = .15f;
	CurrentStamina = MaxStamina = 1.0f;
	StaminaRegenRate = .1f;
	StaminaDepleteRate = .05f;

	IsInteracting = IsSprinting = IsMoving = false;
	CanSprint = true;
	TimerDelay = 2.0f;
}

// Called when the game starts or when spawned
void APirate::BeginPlay() {
	Super::BeginPlay();
	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = BaseSpeed;

	// Get the player controller, though it needs to be casted from AController (getter) to APlayerController
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		// Get the "Enhanced Input Local Player Subsystem" blueprint 
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			// Assign our mapping context to the aforementioned Subsystem
			Subsystem->AddMappingContext(PirateMappingContext, 0);
		}
	}
	
	AnimInstance = GetMesh()->GetAnimInstance();

	
}

// Called every frame
void APirate::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("IsSprinting: %s"), IsSprinting ? TEXT("true") : TEXT("false")));
	if (!IsMoving && CurrentStamina < MaxStamina) {
		CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, MaxStamina, DeltaTime, StaminaRegenRate);
		Cast<APrimaryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateStamina(this);
	}else if (IsMoving && IsSprinting) {
		if (CurrentStamina >= 0) {
			CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, 0, DeltaTime, StaminaDepleteRate);
			Cast<APrimaryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateStamina(this);
			if (CurrentStamina == 0) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SprintCooldown")));
				CanSprint = false;
				SprintCooldown();
				/*
				 * 
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				DisableInput(PlayerController);
				 */
			}
		}
	}else if (IsMoving && !IsSprinting) {
		if (CurrentStamina < MaxStamina) {
			CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, MaxStamina, DeltaTime, StaminaRegenRate);
			Cast<APrimaryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateStamina(this);
		}
	}
}

void APirate::SprintCooldown() {
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APirate::EnableSprinting, TimerDelay, this);
	//TODO: Fix https://www.youtube.com/watch?v=erKB-ufMlPg&ab_channel=DerSky
}


void APirate::EnableSprinting() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CanSprint = true")));
    	CanSprint = true;
}

void APirate::Move(const FInputActionValue& Value) {
	// const bool CurrentValue = Value.Get<FVector2d>();

	// Get the value of our movement, which can be a bool, a float, a FVector2d or FVector (3D)
	const FVector2d DirectionValue = Value.Get<FVector2d>();
	if (GetController()) {
		
		const FVector Forward = GetActorForwardVector();
		UE_LOG(LogTemp, Log, TEXT("Forward vector: %s"), *Forward.ToString());
		const FVector Sideways = GetActorRightVector();
		const FVector Velocity = GetVelocity();
		UE_LOG(LogTemp, Warning, TEXT("DirectionValue: %s"), *DirectionValue.ToString());
		UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *Velocity.ToString());

		IsMoving = true;
		AddMovementInput(Forward, DirectionValue.X * MovementModifier);
		AddMovementInput(Sideways, DirectionValue.Y * MovementModifier);
	}
}

void APirate::MoveCancelled(const FInputActionValue& Value) {
	if (GetController()) {
		UE_LOG(LogTemp, Warning, TEXT("MoveCancelled"));
		IsMoving = false;
	}
}

void APirate::MoveCompleted(const FInputActionValue& Value) {
	if (GetController()) {
		UE_LOG(LogTemp, Warning, TEXT("MoveCompleted"));
		IsMoving = false;
	}
}

void APirate::Look(const FInputActionValue& Value) {
	const FVector2d LookAxisValue = Value.Get<FVector2d>();
	if (GetController()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IA_Look triggered"));
		AddControllerYawInput(LookAxisValue.X);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(LookAxisValue.X));
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void APirate::Jump(const FInputActionValue& Value) {
	if (GetController()) {
		Super::Jump();	
	}
}

void APirate::SprintOngoing(const FInputActionValue& Value) {
	//UE_LOG(LogTemp, Warning, TEXT("IA_Sprint Ongoing"), );
	if (GetController() && IsMoving) {
		MovementModifier = SprintModifier;
		IsSprinting = true;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

void APirate::SprintCanceled(const FInputActionValue& Value) {
	//UE_LOG(LogTemp, Warning, TEXT("IA_Sprint Canceled"), );
	if (GetController() && IsMoving) {
		MovementModifier = WalkModifier;
		IsSprinting = false;
	}
}

void APirate::SprintCompleted(const FInputActionValue& Value) {
	//UE_LOG(LogTemp, Warning, TEXT("IA_Sprint Completed"), );
	if (GetController() && IsMoving) {
		MovementModifier = WalkModifier;
		IsSprinting = false;
	}
}

void APirate::CameraZoom(const FInputActionValue& Value) {
	const FVector DirectionValue = Value.Get<FVector>();
	if (GetController()) {
		if (DirectionValue.X != 0) {
			const float NewTargetArmLenght = CameraBoom->TargetArmLength + (-DirectionValue.X * ZoomStep);
			const FVector TransformedVector = FVector(0, 0, DirectionValue.X);
			// TODO: Camera scroll range disparity may be due to different implementations of FMath::Clamp and ClampVector
			CameraBoom->SetRelativeLocation(ClampVector(CameraBoom->GetRelativeLocation() + (-TransformedVector * ZoomStep / 2), MinCameraHeight, MaxCameraHeight));
			CameraBoom->TargetArmLength = FMath::Clamp(NewTargetArmLenght, MinCameraZoom, MaxCameraZoom);
		}
	}
}

void APirate::StartInteract(const FInputActionValue& Value) {
	if (GetController()) {
		IsInteracting = Value.Get<bool>();
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);
		if (const int NumberElements = OverlappingActors.Num(); NumberElements > 0) {
			UE_LOG(LogTemp, Warning, TEXT("InteractWithOverlap"),);
			MyGameStateBase->InteractWithOverlap(this, OverlappingActors, GetWorld());
		}
	}
}

void APirate::CompletedInteract(const FInputActionValue& Value) {
	if (GetController()) {
		IsInteracting = Value.Get<bool>();
	}
}

void APirate::TestInteraction(const FInputActionValue& Value) {
	MyGameStateBase->TestCounter();
}

void APirate::Attack(const FInputActionValue& Value) {
	if (GetController() && AnimInstance && AnimMontage) {
		const float MontageLength = AnimInstance->Montage_Play(AnimMontage, 1, EMontagePlayReturnType::MontageLength, 0);

		if (MontageLength > 0.f) {
			// TODO: Handle Montage in the future if needed https://forums.unrealengine.com/t/play-montage-in-c-with-onblendout-oninterrupted-etc/447184/2
			
			//TestDelegate.Execute();
		} else {
			//TODO: No idea, do something if it fails. When can an animation fail though?
		}
	}
}


void APirate::EndOfAttackAnimation() {
	UE_LOG(LogTemp, Warning, TEXT("EndOfAttackAnimation"), );
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

void APirate::CanceledInteract(const FInputActionValue& Value) {
	if (GetController()) {
		IsInteracting = Value.Get<bool>();
		UE_LOG(LogTemp, Warning, TEXT("Canceled Interacting Value = %d"), Value.Get<bool>());
	}
}

void APirate::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	// Notifies when hitting the capsule component
	UE_LOG(LogTemp, Warning, TEXT("NotifyActorBeginOverlap from Pirate"),);

}

// Called to bind functionality to input
void APirate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Old method was to bind Axis or Actions here such as:
	 *
	 * InputComponent->BindAxis("Horizontal", this, &AMyCharacter::HorizontalMove);
	 * or
	 * InputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::JumpAction);
	 */
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APirate::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &APirate::MoveCancelled);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APirate::MoveCompleted);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APirate::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APirate::Jump);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Ongoing, this, &APirate::SprintOngoing);
		// TODO: Canceled seems to trigger when sprint is pressed <1 sec and completed >1sec. Investigate. Interestingly, interact does not seem to have this threshold and never hits CanceledInteract (keeping it just in case)
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &APirate::SprintCanceled);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APirate::SprintCompleted);
		EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &APirate::CameraZoom);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APirate::StartInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Canceled, this, &APirate::CanceledInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &APirate::CompletedInteract);
		EnhancedInputComponent->BindAction(TestAction, ETriggerEvent::Completed, this, &APirate::TestInteraction);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APirate::Attack);
	}
}

void APirate::UpdateWidgetHP(APrimaryHUD* PrimaryHUD) {
	PrimaryHUD->UpdateHP(this);
}


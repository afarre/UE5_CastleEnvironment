// Fill out your copyright notice in the Description page of Project Settings.


#include "Pirate.h"

#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CastleEnvironment/MyGameStateBase.h"
#include "CastleEnvironment/UI/PrimaryHUD.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "CastleEnvironment/UI/PauseMenu.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	// Create audio components that will play Sound 
	static ConstructorHelpers::FObjectFinder<USoundCue> EffortGruntCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/SC_Effort_Grunt_Cue.SC_Effort_Grunt_Cue'"));
	if (EffortGruntCueObject.Succeeded()) {
		EffortGrunt = EffortGruntCueObject.Object;
		EffortGruntAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("EffortGruntAudioComponent"));
		EffortGruntAudioComponent->SetupAttachment(RootComponent);
	}
	
	static ConstructorHelpers::FObjectFinder<USoundCue> AttackSlashCueObject(TEXT("/Script/Engine.SoundCue'/Game/Audio/SC_Slash_Cue.SC_Slash_Cue'"));
	if (AttackSlashCueObject.Succeeded()) {
		AttackSlash = AttackSlashCueObject.Object;
		AttackSlashAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AttackSlashAudioComponent"));
		AttackSlashAudioComponent->SetupAttachment(RootComponent);
	}
	
	// Variable inits
	CurrentHealth = MaxHealth = 1.0f;
	MovementModifier = WalkModifier = .25f;
	SprintModifier = 1.0f;
	BaseSpeed = 1200.0f;
	BaseDamage = .15f;
	CurrentStamina = MaxStamina = 1.0f;
	StaminaRegenRate = .1f;
	StaminaDepleteRate = .05f;

	IsPaused = IsInteracting = IsSprinting = IsMoving = AttackConnectedFlag = false;
	CanSprint = true;
	TimerDelay = 2.0f;
}

// Called when the game starts or when spawned
void APirate::BeginPlay() {
	Super::BeginPlay();
	CharacterMovementComponent = GetCharacterMovement();
	CharacterMovementComponent->MaxWalkSpeed = BaseSpeed;
	Controller = Cast<APlayerController>(GetController());
	
	// Get the player controller, though it needs to be casted from AController (getter) to APlayerController
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		// Get the "Enhanced Input Local Player Subsystem" blueprint 
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			// Assign our mapping context to the aforementioned Subsystem
			Subsystem->AddMappingContext(PirateMappingContext, 0);
		}
	}
	//MyGameStateBase = Cast<AMyGameStateBase>(GetWorld()->GetGameState());

	// Init Animation Instance for our Mesh
	AnimInstance = GetMesh()->GetAnimInstance();

	// Audio binds
	EffortGruntAudioComponent->SetSound(EffortGrunt);
	AttackSlashAudioComponent->SetSound(AttackSlash);

	// Share this Pirate instance with the pause menu
	PauseMenu = CreateWidget<UPauseMenu>(Controller, PauseMenuClass, "PauseMenuWN");
	if (PauseMenu) {
		PauseMenu->Pirate = this;
	}
}

// Called every frame
void APirate::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("IsSprinting: %s"), IsSprinting ? TEXT("true") : TEXT("false")));
	if (!IsMoving && CurrentStamina < MaxStamina) {
		CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, MaxStamina, DeltaTime, StaminaRegenRate);
		Cast<APrimaryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateStamina(this);
	}else if (IsMoving && IsSprinting) {
		if (CurrentStamina >= 0) {
			CurrentStamina = FMath::FInterpConstantTo(CurrentStamina, 0, DeltaTime, StaminaDepleteRate);
			Cast<APrimaryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD())->UpdateStamina(this);
			if (CurrentStamina == 0) {
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("SprintCooldown")));
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

// Called to bind functionality to input
void APirate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Old method was to bind Axis or Actions here such as:
	 * InputComponent->BindAxis("Horizontal", this, &AMyCharacter::HorizontalMove);
	 * or
	 * InputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::JumpAction);
	 */
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APirate::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Canceled, this, &APirate::MoveEnd);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &APirate::MoveEnd);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APirate::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APirate::Jump);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Ongoing, this, &APirate::SprintOngoing);
		// TODO: Canceled seems to trigger when sprint is pressed <1 sec and completed >1sec. Investigate. Interestingly, interact does not seem to have this threshold and never hits CanceledInteract (keeping it just in case)
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Canceled, this, &APirate::SprintEnd);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APirate::SprintEnd);
		EnhancedInputComponent->BindAction(WheelAction, ETriggerEvent::Triggered, this, &APirate::CameraZoom);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APirate::StartInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Canceled, this, &APirate::InteractEnd);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &APirate::InteractEnd);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APirate::Attack);
		EnhancedInputComponent->BindAction(MenuAction, ETriggerEvent::Triggered, this, &APirate::Menu);
	}
}

/**
 * @brief Adds movement to our character based on the player's input
 * @param Value Input value received from the player (WASD keys), a 2D vector for this particular Input Action
 */
void APirate::Move(const FInputActionValue& Value) {
	// const bool CurrentValue = Value.Get<FVector2d>();

	// Get the value of our movement, which can be a bool, a float, a FVector2d or FVector (3D)
	const FVector2d DirectionValue = Value.Get<FVector2d>();
	if (GetController()) {
		
		const FVector Forward = GetActorForwardVector();
		const FVector Sideways = GetActorRightVector();

		IsMoving = true;
		AddMovementInput(Forward, DirectionValue.X * MovementModifier);
		AddMovementInput(Sideways, DirectionValue.Y * MovementModifier);
		//UE_LOG(LogTemp, Log, TEXT("Forward vector: %s"), *Forward.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("DirectionValue: %s"), *DirectionValue.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *GetVelocity().ToString());
	}
}

/**
 * @brief Called when the move input action is cancelled
 */
void APirate::MoveEnd() {
	if (GetController()) {
		//UE_LOG(LogTemp, Warning, TEXT("MoveCompleted"));
		IsMoving = false;
	}
}

/**
 * @brief Handles the movement of the mouse, which in turn moves the character's camera
 * @param Value Input value received from the player (mouse movement), a 2D vector for this particular Input Action
 */
void APirate::Look(const FInputActionValue& Value) {
	const FVector2d LookAxisValue = Value.Get<FVector2d>();
	if (GetController()) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IA_Look triggered"));
		AddControllerYawInput(LookAxisValue.X);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::SanitizeFloat(LookAxisValue.X));
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

/**
 * @brief Calls ACharacter's jump function
 */
void APirate::Jump() {
	if (GetController()) {
		Super::Jump();	
	}
}

/**
 * @brief Called as long as our character is pressing the sprint key
 */
void APirate::SprintOngoing() {
	//UE_LOG(LogTemp, Warning, TEXT("IA_Sprint Ongoing"), );
	if (GetController() && IsMoving) {
		MovementModifier = SprintModifier;
		IsSprinting = true;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}
}

/**
 * @brief Called when the sprinting input action is cancelled
 */
void APirate::SprintEnd() {
	//UE_LOG(LogTemp, Warning, TEXT("IA_Sprint Canceled"), );
	if (GetController() && IsMoving) {
		MovementModifier = WalkModifier;
		IsSprinting = false;
	}
}

/**
 * @brief Enables sprinting
 */
void APirate::EnableSprinting() {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CanSprint = true")));
	CanSprint = true;
}

/**
 * @brief Handles the camera zoom when the player uses the mouse's scroll wheel
 * @param Value Input value received from the player (scroll wheel), a vector for this particular Input Action
 */
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

/**
 * @brief Handles when the player presses the E key to interact with objects/people
 * @param Value Input value received from the player (E key), a boolean for this particular Input Action
 */
void APirate::StartInteract(const FInputActionValue& Value) {
	if (GetController()) {
		IsInteracting = Value.Get<bool>();
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);
		if (const int NumberElements = OverlappingActors.Num(); NumberElements > 0) {
			//UE_LOG(LogTemp, Warning, TEXT("InteractWithOverlap"),);
			MyGameStateBase->InteractWithOverlap(this, OverlappingActors, GetWorld());
		}
	}
}

/**
 * @brief Called when the interacting input action is completed
 * @param Value Input value received from the player (E key), a boolean for this particular Input Action
 */
void APirate::InteractEnd(const FInputActionValue& Value) {
	if (GetController()) {
		IsInteracting = Value.Get<bool>();
	}
}

/**
 * @brief Calls the HUD in order for it to be aware that the character's HP widget must be updated
 * @param PrimaryHUD A reference to this character's HUD
 */
void APirate::UpdateWidgetHP(APrimaryHUD* PrimaryHUD) {
	PrimaryHUD->UpdateHP(this);
}


/**
 * @brief Trriggered when the character attacks, and thus initiates the attack animation
 */
void APirate::Attack() {
	if (GetController() && AnimInstance && SlashAttackAnimMontage && WieldedWeapon) {
		if (const float MontageLength = PlayAnimMontage(SlashAttackAnimMontage, 1.0f); MontageLength <= 0.f) {
			//TODO: No idea, do something if it fails. When can an animation fail though? When interrupted by another action such as taking damage perhaps?
		}
	}
}

/**
 * @brief Handle when the player presses the escape key to access the menu
 */
void APirate::Menu() {
	if (GetController()) {
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		//we use IsLocallyControlled() because we want to create this widget if there is a user on screen connected to this pawn as opposed to create it for remotely controlled pawn
		if (IsPaused && PauseMenuClass && IsLocallyControlled() && PlayerController && PauseMenu) {
			// Disable cursor
			Controller->bShowMouseCursor = false; 
			Controller->bEnableClickEvents = false; 
			Controller->bEnableMouseOverEvents = false;

			// Unpause and resume game
			UGameplayStatics::SetGamePaused(GetWorld(),false);
			PauseMenu->RemoveFromParent();
			IsPaused = false;
		}else if (!IsPaused && PauseMenuClass && IsLocallyControlled() && PlayerController && PauseMenu){
			// Enable cursor
			Controller->bShowMouseCursor = true; 
			Controller->bEnableClickEvents = true; 
			Controller->bEnableMouseOverEvents = true;

			// Pause and show menu
			UGameplayStatics::SetGamePaused(GetWorld(),true);
			PauseMenu->AddToViewport();
			IsPaused = true;
		}
	}
}

/**
 * @brief Records if the attack from the character has connected with another actor
 * @param AttackConnectedActor Actor that received the character's attack 
 */
void APirate::AttackConnected(AActor* AttackConnectedActor) {
	AttackConnectedFlag = true;
	if (!ConnectedEnemies.Contains(AttackConnectedActor)) {
		ConnectedEnemies.Add(AttackConnectedActor);
	}
}

/**
 * @brief Iterates over actors that received a damaging attack and updates their HP accordingly.
 */
void APirate::ActOnConnectedAttacks() {
	if (AttackConnectedFlag) {
		for (AActor* ConnectedEnemy : ConnectedEnemies) {
			MyGameStateBase->TakeDamage(ConnectedEnemy, WieldedWeapon->BaseDamage);
		}
		ConnectedEnemies.Empty();
	}
}


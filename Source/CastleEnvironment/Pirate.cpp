// Fill out your copyright notice in the Description page of Project Settings.


#include "Pirate.h"

#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "VectorTypes.h"
#include "WarMace.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	Interacting = false;
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
}

void APirate::Move(const FInputActionValue& Value) {
	// const bool CurrentValue = Value.Get<FVector2d>();

	// Get the value of our movement, which can be a bool, a float, a FVector2d or FVector (3D)
	const FVector2d DirectionValue = Value.Get<FVector2d>();
	if (GetController()) {
		
		//UE_LOG(LogTemp, Warning, TEXT("IA_Move triggered"));
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("IA_Move triggered (screen)"));
		//UE_LOG(LogTemp, Log, TEXT("Movement vector: %s"), *DirectionValue.ToString());
		
		const FVector Forward = GetActorForwardVector();
		//UE_LOG(LogTemp, Log, TEXT("Forward vector: %s"), *Forward.ToString());
		const FVector Sideways = GetActorRightVector();
		//const FVector Velocity = GetVelocity();
		//UE_LOG(LogTemp, Log, TEXT("Velocity: %s"), *Velocity.ToString());
		AddMovementInput(Forward, DirectionValue.X * MovementModifier);
		AddMovementInput(Sideways, DirectionValue.Y * MovementModifier);
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
	UE_LOG(LogTemp, Warning, TEXT("IA_Sprint Ongoing"), );
	if (GetController()) {
		MovementModifier = SprintModifier;
	}
}

void APirate::SprintCanceled(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Warning, TEXT("IA_Sprint Canceled"), );
	if (GetController()) {
		MovementModifier = WalkModifier;
	}
}

void APirate::SprintCompleted(const FInputActionValue& Value) {
	UE_LOG(LogTemp, Warning, TEXT("IA_Sprint Completed"), );
	if (GetController()) {
		MovementModifier = WalkModifier;
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
			UE_LOG(LogTemp, Warning, TEXT("Interacting value = %d"), Interacting);
		}
	}
}

void APirate::StartInteract(const FInputActionValue& Value) {
	if (GetController()) {
		Interacting = Value.Get<bool>();
		TArray<AActor*> OverlappingActors;
		GetOverlappingActors(OverlappingActors);
		if (const int NumberElements = OverlappingActors.Num(); NumberElements > 0) {
			UE_LOG(LogTemp, Warning, TEXT("InteractWithOverlap"),);
			MyGameStateBase->InteractWithOverlap(this, OverlappingActors, GetWorld());
		}
		//const AWarMace* AWarMAce = Cast<class AWarMace>(GetWorld()->GetFirstPlayerController()->GetCharacter());;
	}
}

void APirate::CompletedInteract(const FInputActionValue& Value) {
	if (GetController()) {
		Interacting = Value.Get<bool>();
		UE_LOG(LogTemp, Warning, TEXT("Completed Interacting Value = %d"), Value.Get<bool>());
	}
}

void APirate::TestInteraction(const FInputActionValue& Value) {
	MyGameStateBase->TestCounter();
}

void APirate::CanceledInteract(const FInputActionValue& Value) {
	if (GetController()) {
		Interacting = Value.Get<bool>();
		UE_LOG(LogTemp, Warning, TEXT("Canceled Interacting Value = %d"), Value.Get<bool>());
	}
}

// Called every frame
void APirate::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
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
	}
}


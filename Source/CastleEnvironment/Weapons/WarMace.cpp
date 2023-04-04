// Fill out your copyright notice in the Description page of Project Settings.

#include "WarMace.h"

#include "CastleEnvironment/MyGameStateBase.h"
#include "CastleEnvironment/Characters/Pirate.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Forward declaration
class APirate;

// Sets default values
AWarMace::AWarMace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	PickupCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Pickup collision Box"));
	PickupCollisionBox->SetupAttachment(StaticMesh);
	
	WeaponCollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Weapon collision Box"));
	WeaponCollisionCapsule->SetupAttachment(RootComponent);
	WeaponCollisionCapsule->SetCapsuleRadius(15);
	//WeaponCollisionCapsule->SetHiddenInGame(false);
	
	HasBeenPickedUp = HasOverlapped = IsWielded = false;
}

// Called when the game starts or when spawned
void AWarMace::BeginPlay() {
	Super::BeginPlay();
	
	// Attach collision components to sockets based on transformations definitions
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld, false);
	WeaponCollisionCapsule->AttachToComponent(StaticMesh, AttachmentRules, "DamageCollisionSocket");
	WeaponCollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AWarMace::BeginOverlap);
	WeaponCollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AWarMace::EndOverlap);
	WeaponCollisionCapsule->SetNotifyRigidBodyCollision(true);
	WeaponCollisionCapsule->SetCollisionProfileName(MeleeCollisionProfile.Disabled);
}

// Called every frame
void AWarMace::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AWarMace::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult) {
	// TODO: I profoundly despise how attacks and overlap are implemented, but it works and as such it will remain as is for now.
	HasOverlapped = true;
	OverlappedActor = OtherActor;
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap with %s"), *OtherActor->GetName());
}

void AWarMace::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	HasOverlapped = false;
	OverlappedActor = nullptr;
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));
}

void AWarMace::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!HasBeenPickedUp) {
		MyGameStateBase->DisplayPrompt();
		HasOverlapped = true;
		OverlappedActor = OtherActor;
		UE_LOG(LogTemp, Warning, TEXT("NotifyActorBeginOverlap"));
	}
}

void AWarMace::WeaponPickedUp() const{
	PickupCollisionBox->Deactivate();
	HasBeenPickedUp = true;
}

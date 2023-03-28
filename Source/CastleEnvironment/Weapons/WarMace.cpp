// Fill out your copyright notice in the Description page of Project Settings.

#include "WarMace.h"

#include "CastleEnvironment/MyGameStateBase.h"
#include "CastleEnvironment/Characters/Pirate.h"
#include "CastleEnvironment/Characters/SkeletonSwordsman.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

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
	WeaponCollisionCapsule->SetupAttachment(StaticMesh);
	WeaponCollisionCapsule->OnComponentHit.AddDynamic(this, &AWarMace::WarMaceHit);
	AttackEnd(WeaponCollisionCapsule);
	
	IsWielded = false;
}

// Called when the game starts or when spawned
void AWarMace::BeginPlay() {
	Super::BeginPlay();
}

void AWarMace::WarMaceHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit) {
	if (IsWielded) {
		if (Cast<ASkeletonSwordsman>(OtherActor)) {
			//MyGameStateBase->HitSkeleton(BaseDamage, GetWorld());
			UE_LOG(LogTemp, Warning, TEXT("War mace hit with skeleton"),);
		}
	}
}

void AWarMace::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	// Notifies when hitting the capsule component
	//UE_LOG(LogTemp, Warning, TEXT("NotifyActorBeginOverlap weapon"),);
	MyGameStateBase->DisplayPrompt();
}

void AWarMace::DisablePickupCollisionBox() const {
	PickupCollisionBox->Deactivate();
}

// Called every frame
void AWarMace::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

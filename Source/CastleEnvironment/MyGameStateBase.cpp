// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

#include "Components/CapsuleComponent.h"
#include "ParentCharacter.h"
#include "Pirate.h"
#include "WarMace.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// Called when the game starts or when spawned
void AMyGameStateBase::BeginPlay()
{
	Super::BeginPlay();


}

void AMyGameStateBase::TakeDamage(AParentCharacter* Actor, float DamageAmount) const {
	if (Actor) {
		Actor->CurrentHealth = Actor->CurrentHealth - DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Damage taken: %f"), DamageAmount);
		if (Actor->CurrentHealth <= 0) {
			// Ragdoll if pirate
			APirate* Pirate = Cast<APirate>(Actor);
			if (Pirate) {
				Pirate->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                Pirate->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				Pirate->GetMesh()->SetSimulatePhysics(true);
			}else {
				// TODO: Implement death of non-Pirate characters, ie. enemies
			}
		}
	}
}

void AMyGameStateBase::InteractWithOverlap(APirate* Pirate, TSet<AActor*> OverlappingActors, UWorld* World) const {
	// Fetch all actors of class AWarMace
	TArray<AActor*> ActorsToFind;
	if (World) {
		UE_LOG(LogTemp, Warning, TEXT("got world"),);
	
		UGameplayStatics::GetAllActorsOfClass(World, AWarMace::StaticClass(), ActorsToFind);

		// Iterate over each actor, casting it to AWarMace
		for (AActor* Actor: ActorsToFind) {
			if (const AWarMace* WarMace = Cast<AWarMace>(Actor)) {
				// Check if the item we are iterating over is overlapping with the pirate
				if (OverlappingActors.Contains(WarMace)) {
					UE_LOG(LogTemp, Warning, TEXT("I'm interacting with a War mace"),);
				
					WarMace->StaticMesh->SetSimulatePhysics(false);
					WarMace->StaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
					WarMace->StaticMesh->AttachToComponent(Pirate->GetMesh(),  FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("LeftWeapon"));
				}
			}
		}
	}
}

void AMyGameStateBase::DisplayPrompt() {

	// TODO: Display "Press E to pick up" in HUD
}

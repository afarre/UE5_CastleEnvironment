// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "Characters/ParentEnemy.h"
#include "Characters/Pirate.h"
#include "UI/PrimaryHUD.h"
#include "Weapons/WarMace.h"


void AMyGameStateBase::TakeDamage(AActor* Actor, float DamageAmount) const {
	APirate* Pirate = Cast<APirate>(Actor);
	AParentEnemy* ParentEnemy = Cast<AParentEnemy>(Actor);
	if (GetWorld()) {
		UE_LOG(LogTemp, Warning, TEXT("got world in take damage"),);
	}
	if (Pirate) {
		UE_LOG(LogTemp, Warning, TEXT("Pirate damage taken: %f"), DamageAmount);
		Pirate->CurrentHealth = Pirate->CurrentHealth - DamageAmount;
		OnPlayerTakeDamage(Pirate);
	} else if (ParentEnemy) {
		UE_LOG(LogTemp, Warning, TEXT("Enemy amage taken: %f"), DamageAmount);
		ParentEnemy->CurrentHealth = ParentEnemy->CurrentHealth - DamageAmount;
		OnEnemyTakeDamage(ParentEnemy);
	}
}

void AMyGameStateBase::InteractWithOverlap(APirate* Pirate, TArray<AActor*> OverlappingActors, UWorld* World) const {
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

void AMyGameStateBase::TestCounter() {
	
}

void AMyGameStateBase::OnPlayerTakeDamage(APirate* Pirate) const {
	APrimaryHUD* PrimaryHUD = Cast<APrimaryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	Pirate->UpdateWidgetHP(PrimaryHUD);
	
	if (Pirate->CurrentHealth <= 0) {
		// Ragdoll
		Pirate->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Pirate->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Pirate->GetMesh()->SetSimulatePhysics(true);
			
		if (PrimaryHUD) {
			PrimaryHUD->FadeHealthBarAnimation();
		}
	}
}

void AMyGameStateBase::OnEnemyTakeDamage(AParentEnemy* ParentEnemy) const {
	ParentEnemy->UpdateHP();
	if (ParentEnemy->CurrentHealth <= 0) {
		// TODO: Ragdoll enemy
	}
}

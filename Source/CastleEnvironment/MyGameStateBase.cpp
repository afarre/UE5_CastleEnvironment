// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "Characters/ParentEnemy.h"
#include "Characters/Pirate.h"
#include "Characters/SkeletonSwordsman.h"
#include "Objects/ParentObject.h"
#include "UI/PrimaryHUD.h"
#include "Weapons/WarMace.h"

void AMyGameStateBase::BeginPlay() {
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AParentWeapon::StaticClass(), ParentWeaponArray);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AParentObject::StaticClass(), ParentObjectArray);
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AParentEnemy::StaticClass(), ParentEnemyArray);
}


void AMyGameStateBase::TakeDamage(AActor* Actor, float DamageAmount) const {
	APirate* Pirate = Cast<APirate>(Actor);
	AParentEnemy* ParentEnemy = Cast<AParentEnemy>(Actor);

	if (Cast<APirate>(Actor)) {
		UE_LOG(LogTemp, Warning, TEXT("Pirate damage taken: %f"), DamageAmount);
		Pirate->CurrentHealth = Pirate->CurrentHealth - DamageAmount;
		OnPlayerTakeDamage(Pirate);
	} else if (ParentEnemy) {
		UE_LOG(LogTemp, Warning, TEXT("Enemy damage taken: %f"), DamageAmount);
		ParentEnemy->CurrentHealth = ParentEnemy->CurrentHealth - DamageAmount;
		OnEnemyTakeDamage(ParentEnemy);
	}
}

void AMyGameStateBase::InteractWithOverlap(APirate* Pirate, TArray<AActor*> OverlappingActors, UWorld* World) {
	// Fetch all actors of class AWarMace
	TArray<AActor*> ActorsToFind;
	if (World) {
		UGameplayStatics::GetAllActorsOfClass(World, AWarMace::StaticClass(), ActorsToFind);

		// Iterate over each actor, casting it to AWarMace
		for (AActor* Actor: ActorsToFind) {
			if (const AWarMace* WarMace = Cast<AWarMace>(Actor)) {
				// Check if the item we are iterating over is overlapping with the pirate
				if (OverlappingActors.Contains(WarMace)) {
					UE_LOG(LogTemp, Warning, TEXT("I'm interacting with a War mace"),);

					WarMace->DisablePickupCollisionBox();
					WarMace->StaticMesh->SetSimulatePhysics(false);
					WarMace->StaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
					WarMace->StaticMesh->AttachToComponent(Pirate->GetMesh(),  FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("RightWeapon"));
					WarMace->IsWielded = true;
					Pirate->WieldedWeapon = WarMace;
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

void AMyGameStateBase::Attacking(APirate* Pirate, UWorld* World) {
	if (Pirate->WieldedWeapon) {
		Pirate->WieldedWeapon->AttackStart(Pirate->WieldedWeapon->WeaponCollisionCapsule);
		UE_LOG(LogTemp, Warning, TEXT("Called Attack start from game state base"),);
	}
	/*
	 * 
	TArray<AActor*> OverlappingActors;
	if (World) {
		if (Pirate->WieldedWeapon) {
	        Pirate->WieldedWeapon->GetOverlappingActors(OverlappingActors);
	        if (const int NumberElements = OverlappingActors.Num(); NumberElements > 0) {
        		for (AActor* Actor: ParentEnemyArray) {
		            if (const ASkeletonSwordsman* SkeletonSwordsman = Cast<ASkeletonSwordsman>(Actor)) {
        				if (OverlappingActors.Contains(SkeletonSwordsman)) {
        					UE_LOG(LogTemp, Warning, TEXT("My attack is overlapping with a skeleton"),);
        				}
		            }
        		}
	        }
		}
	}
	 */
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

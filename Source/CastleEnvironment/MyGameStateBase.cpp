// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/WidgetAnimation.h"
#include "Characters/ParentEnemy.h"
#include "Characters/Pirate.h"
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
				if (OverlappingActors.Contains(WarMace) && !WarMace->IsWielded) {
					UE_LOG(LogTemp, Warning, TEXT("I'm interacting with a War mace"),);

					WarMace->WeaponPickedUp();
					WarMace->StaticMesh->SetSimulatePhysics(false);
					WarMace->StaticMesh->AttachToComponent(Pirate->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), TEXT("RightWeapon"));
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

void AMyGameStateBase::OnPlayerTakeDamage(APirate* Pirate) const {
	APrimaryHUD* PrimaryHUD = Cast<APrimaryHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (Pirate->CurrentHealth <= 0) {
		// Ragdoll
		Pirate->CurrentHealth = 0;
		Pirate->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Pirate->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Pirate->GetMesh()->SetSimulatePhysics(true);
			
		if (PrimaryHUD) {
			PrimaryHUD->FadeHealthBarAnimation();
		}
	}
	Pirate->UpdateWidgetHP(PrimaryHUD);
}

void AMyGameStateBase::OnEnemyTakeDamage(AParentEnemy* ParentEnemy) const {
	if (ParentEnemy->CurrentHealth <= 0) {
		ParentEnemy->CurrentHealth = 0;
		ParentEnemy->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		ParentEnemy->GetMesh()->SetSimulatePhysics(true);
		ParentEnemy->SetActorEnableCollision(false);
			
		if (ParentEnemy) {
			ParentEnemy->HealthBar->FadeHealthBarAnimation();
		}
	}
	ParentEnemy->UpdateHP();
}

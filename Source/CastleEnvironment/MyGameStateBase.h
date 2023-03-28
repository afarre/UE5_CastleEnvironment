// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Weapons/WarMace.h"
#include "MyGameStateBase.generated.h"

class AParentEnemy;
class APirate;

/**
 * 
 */
UCLASS()
class CASTLEENVIRONMENT_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	
public:
	void TakeDamage(AActor* Actor, float DamageAmount) const;
	void InteractWithOverlap(APirate* Pirate, TArray<AActor*> OverlappingActors, UWorld* World);
	void DisplayPrompt();
	void TestCounter();
	void Attacking(APirate* Pirate, UWorld* World);

	virtual void BeginPlay() override;

private:
	void OnPlayerTakeDamage(APirate* Pirate) const;
	void OnEnemyTakeDamage(AParentEnemy* ParentEnemy) const;

	TArray<AActor*> ParentWeaponArray;
	TArray<AActor*> ParentObjectArray;
	TArray<AActor*> ParentEnemyArray;
	
};

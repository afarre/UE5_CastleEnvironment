// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

class AParentCharacter;
class APirate;

/**
 * 
 */
UCLASS()
class CASTLEENVIRONMENT_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

	
public:
	void TakeDamage(AParentCharacter* Actor, float DamageAmount) const;
	void InteractWithOverlap(APirate* Pirate, TArray<AActor*> OverlappingActors, UWorld* World) const;
	void DisplayPrompt();
	void TestCounter();
};

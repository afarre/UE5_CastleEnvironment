// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParentWeapon.generated.h"

class AMyGameStateBase;

UCLASS()
class CASTLEENVIRONMENT_API AParentWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AMyGameStateBase* MyGameStateBase;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentCharacter.h"
#include "GameFramework/Character.h"
#include "SkeletonSwordsman.generated.h"

UCLASS()
class CASTLEENVIRONMENT_API ASkeletonSwordsman : public AParentCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASkeletonSwordsman();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

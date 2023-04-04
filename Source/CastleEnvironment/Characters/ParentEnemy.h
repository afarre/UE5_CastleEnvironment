// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ParentEnemy.generated.h"

class UWidgetComponent;
class AMyGameStateBase;
class UHealthBar;

UCLASS()
class CASTLEENVIRONMENT_API AParentEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AParentEnemy();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void UpdateHP() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	mutable float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	mutable float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Health)
	mutable float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	mutable float BaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	mutable float WalkModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	mutable float SprintModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement)
	mutable float MovementModifier;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthWidgetComponent;

	UHealthBar* HealthBar;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	AMyGameStateBase* MyGameStateBase;
};

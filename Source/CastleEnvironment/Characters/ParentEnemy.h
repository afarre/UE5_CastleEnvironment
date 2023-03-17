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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	mutable float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	mutable float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	AMyGameStateBase* MyGameStateBase;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void UpdateHP() const;

private:
	UHealthBar* HealthBar;
	
};

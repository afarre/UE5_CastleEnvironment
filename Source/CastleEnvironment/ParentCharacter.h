// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyGameStateBase.h"
#include "ParentCharacter.generated.h"

class UWidgetComponent;
class AMyGameStateBase;

UCLASS()
class CASTLEENVIRONMENT_API AParentCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AParentCharacter();

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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthWidget;

	AMyGameStateBase* MyGameStateBase;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParentWeapon.generated.h"

USTRUCT(BlueprintType)
struct FMeleeCollisionProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Enabled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Disabled;

	// default constructor
	FMeleeCollisionProfile()
	{
		Enabled = FName(TEXT("Weapon"));
		Disabled = FName(TEXT("NoCollision"));
	}
};

class UCapsuleComponent;
class AMyGameStateBase;

UCLASS()
class CASTLEENVIRONMENT_API AParentWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParentWeapon();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AttackEnd(UCapsuleComponent* WeaponCollisionCapsule) const;
	void AttackStart(UCapsuleComponent* WeaponCollisionCapsule) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AMyGameStateBase* MyGameStateBase;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	float BaseDamage = .1f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Stats)
	float BaseStaminaConsumption = .05f;

	FMeleeCollisionProfile MeleeCollisionProfile;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentWeapon.h"
#include "WarMace.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AParentEnemy;
class AParentWeapon;
class AParentObject;
class UCapsuleComponent;

UCLASS()
class CASTLEENVIRONMENT_API AWarMace : public AParentWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarMace();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void WeaponPickedUp() const;

	mutable bool IsWielded;
	
	// Create the collision capsule
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCapsuleComponent* WeaponCollisionCapsule;

	bool HasOverlapped;
	AActor* OverlappedActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Create the collision capsule
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* PickupCollisionBox;
	
	// Functions
	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bBFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	mutable bool HasBeenPickedUp;
};

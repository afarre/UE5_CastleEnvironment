// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentWeapon.h"
#include "WarMace.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class AParentCharacter;
class AParentWeapon;
class AParentObject;

UCLASS()
class CASTLEENVIRONMENT_API AWarMace : public AParentWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWarMace();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Create the collision capsule
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;
	
	// Functions
	UFUNCTION()
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

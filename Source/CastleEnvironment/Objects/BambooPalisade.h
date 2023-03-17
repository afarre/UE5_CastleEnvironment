// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ParentObject.h"
#include "BambooPalisade.generated.h"

// Forward declarations
class UBoxComponent;
class UStaticMeshComponent;
class AMyGameStateBase;

UCLASS()
class CASTLEENVIRONMENT_API ABambooPalisade : public AParentObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABambooPalisade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Create the collision capsule
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	// Detect collisions
	UFUNCTION()
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	
	UFUNCTION()
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};

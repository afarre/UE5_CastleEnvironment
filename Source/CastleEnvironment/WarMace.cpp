// Fill out your copyright notice in the Description page of Project Settings.

#include "WarMace.h"


#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Forward declaration
class APirate;

// Sets default values
AWarMace::AWarMace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetupAttachment(CollisionBox);

}

// Called when the game starts or when spawned
void AWarMace::BeginPlay()
{
	Super::BeginPlay();
	
}


void AWarMace::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) {
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	// Notifies when hitting the mesh
	UE_LOG(LogTemp, Warning, TEXT("NotifyHit"),);
}

void AWarMace::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	// Notifies when hitting the capsule component
	UE_LOG(LogTemp, Warning, TEXT("NotifyActorBeginOverlap weapon"),);

	APirate* APirate = Cast<class APirate>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	// TODO: Display "Press E to pick up" in HUD
	if (APirate->Interacting) {
		StaticMesh->SetSimulatePhysics(false);
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		StaticMesh->AttachToComponent(APirate->GetMesh(),  FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("LeftWeapon"));
	} else {
		
	}

}

// Called every frame
void AWarMace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


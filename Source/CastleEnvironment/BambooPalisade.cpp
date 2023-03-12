// Fill out your copyright notice in the Description page of Project Settings.


#include "BambooPalisade.h"

#include "Pirate.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABambooPalisade::ABambooPalisade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(StaticMesh);

}

// Called when the game starts or when spawned
void ABambooPalisade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABambooPalisade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABambooPalisade::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) {
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);
	// Notifies when hitting the mesh
	UE_LOG(LogTemp, Warning, TEXT("NotifyHit"),);
}


void ABambooPalisade::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
	// Notifies when hitting the capsule component
	UE_LOG(LogTemp, Warning, TEXT("NotifyActorBeginOverlap"),);
	TakeDamage(.5f);
}

void ABambooPalisade::TakeDamage(float DamageAmount) {
	const APirate* APirate = Cast<class APirate>(GetWorld()->GetFirstPlayerController()->GetCharacter());;
	if (APirate) {
		APirate->CurrentHealth = APirate->CurrentHealth - DamageAmount;
		UE_LOG(LogTemp, Warning, TEXT("Damage taken: %f"), DamageAmount);
		if (APirate->CurrentHealth <= 0) {
			// Ragdoll
			APirate->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			APirate->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			APirate->GetMesh()->SetSimulatePhysics(true);
		}
	}
}


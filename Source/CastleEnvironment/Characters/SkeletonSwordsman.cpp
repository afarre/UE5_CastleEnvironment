// Fill out your copyright notice in the Description page of Project Settings.


#include "SkeletonSwordsman.h"

// Sets default values
ASkeletonSwordsman::ASkeletonSwordsman() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CurrentHealth = MaxHealth = 1.0f;
	MovementModifier = WalkModifier = .25f;
	SprintModifier = .95f;
	BaseSpeed = 1200.0f;
	BaseDamage = .1f;
}

// Called when the game starts or when spawned
void ASkeletonSwordsman::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ASkeletonSwordsman::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASkeletonSwordsman::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASkeletonSwordsman::ReduceHp(float DamageTaken) {
	CurrentHealth = CurrentHealth - DamageTaken;
}
